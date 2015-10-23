<?php

/**
 * @file
 * Handles objects and links import operation.
 *
 * @todo split objects and links fetch operation in separate
 * processes, when MaPS System® will be able to deliver 2
 * separate streams.
 */

namespace Drupal\maps_import\Fetcher;

use Drupal\maps_import\Exception\Exception as ImportException;
use Drupal\maps_import\Request\Request;
use Drupal\maps_import\Profile\Profile;
use Drupal\maps_suite\Log\Logger;

/**
 * MaPS Import Fetcher class related to objects.
 */
class Objects extends Fetcher {

  /**
   * @inheritdoc
   */
  public function __construct(Profile $profile) {
    parent::__construct($profile);

    // Differential call.
    if (variable_get('maps_import_differential', 0)) {
      if ($min_updated_date = variable_get('maps_import:objects_diff:' . $this->getProfile()->getPid(), FALSE)) {
        $this->setArguments(array('minUpdatedDate' => $min_updated_date));
      }
    }
  }

  /**
   * @inheritdoc
   */
  public function getTitle() {
    return t('Objects import');
  }

  /**
   * @inheritdoc
   */
  public function getDescription() {
    return t('The objects import operation is related to the profile %profile. It first performs some requests against MaPS System® Web services to get a list of objects, media and object links. Then it parses the fetched data and finally saves them in their dedicated tables into the database.', array('%profile' => $this->getProfile()->getTitle()));
  }

  /**
   * @inheritdoc
   */
  public function getType() {
    return 'objects_fetch';
  }

  /**
   * Get the total number of operations.
   *
   * @return int
   *   The total number of operations.
   */
  public function getTotalOperations(array $args = array()) {
    $this->setTimeout(self::TIMEOUT);
    $args['noData'] = 1;
    $data = Request::getData($this->getProfile(), $args, $this->options);

    if (variable_get('maps_import_differential', 0)) {
      if ($min_updated_date = variable_get('maps_import:objects_diff:' . $this->getProfile()->getPid(), FALSE)) {
        $args['minUpdatedDate'] = $min_updated_date;
      }
    }

    // @todo take care of "in_file_links" when available.
    return isset($data['info']['in_file_objects']['value']) ? $data['info']['in_file_objects']['value'] : 0;
  }

  /**
   * @inheritdoc/
   */
  public function batchOperations() {
    $operations = array();

    // Get the total number of objects to fetch by requesting
    // MaPS System® Web Services.
    $total = $this->getTotalOperations();
    $max_objects_per_request = $this->getProfile()->getMaxObjectsPerRequest();

    if ($total && !empty($max_objects_per_request)) {
      $count = ceil($total / $max_objects_per_request);

      for ($i = 0; $i < $count; $i++) {
        $operations[] = array(
          // Callback function
          array($this, 'process'),
          // Callback arguments
          array(
            $i * $max_objects_per_request,
            $max_objects_per_request,
            // The last operation must take care of objects that may be updated
            // after the initial count request. So this last parameter informs
            // the process method that it have to check for remaining objects
            // and leave the queue item (or the batch operation) unfinished.
            (bool) ($i == ($count - 1)),
          ),
        );
      }//end for
    }

    return $operations;
  }

  /**
   * Fetches then saves the data.
   *
   * @param $index
   *   The index from which retrieve objects.
   * @param $range
   *   The maximum number of objects to retrieve.
   * @param $last
   *   Flag indicating if the current operation is the last one of the
   *   serie. If so, some extra checks are performed, which may result
   *   in re-queueing the operation.
   */
  protected function processRun($index = 0, $range = 0, $last = FALSE) {
    $fetchMethod = $this->getProfile()->getFetchMethod();
    $current_context = & $this->context['results'][$this->getType()];

    if (!isset($current_context['total_op'])) {
      $current_context['total_op'] = ceil($this->getTotalOperations() / $range);
    }

    if ($last && isset($current_context['index'])) {
      $index = $current_context['index'];
    }

    if ($range > 0) {
	    $this->setArguments(array(
	      'startIndex' => $index,
	      'maxResults' => $range,
	    ));

      // @check Does not seems pretty accurate...
      // if ($fetchMethod === Profile::FETCH_WS) {
      //   $total = $this->getTotalOperations(array('startIndex' => $index, 'maxResults' => $range));
      //   Logger::getLog($this->getType())->setTotalOperations($current_context['total_op']);
      // }

      Logger::getLog($this->getType())->setCurrentOperation(ceil($index / $range) + 1);
    }
    else {
    	$this->setArguments(array(
    	  'startIndex' => $index,
      ));

      if ($fetchMethod === Profile::FETCH_WS) {
        $total = $this->getTotalOperations(array('startIndex' => $index));
      }

      Logger::getLog($this->getType())->setCurrentOperation(1);
    	Logger::getLog($this->getType())->setTotalOperations(1);
    }

    // Message displayed under the progressbar.
    $this->context['message'] = t('Fetching objects for profile "@title" (@current/@total)', array(
      '@title' => $this->getProfile()->getTitle(),
      '@current' => $current_context['processed_op'],
      '@total' => $current_context['total_op'],
    ));

    $return = parent::processRun($index, $range, $last);

    // @check Does not seems pretty accurate...
    // if ($fetchMethod === Profile::FETCH_WS && !empty($total)) {
    //   $last = FALSE;
    //   $this->context['results'][$this->getType()]['index'] = $index;
    //   $coeff = $range > $total ? 0.5 : $range / $total;
    //   $this->context['finished'] = ($current_context['total_op'] - 1 + $coeff) / $current_context['total_op'];
    //   Logger::getLog($this->getType())->setTotalOperations($current_context['processed_op']);
    // }

    Logger::getLog($this->getType())->save();

    if ($last || empty($total)) {
      $this->updateImportDate();
    }

    return $return;
  }

  /**
   * @inheritdoc
   */
  protected function save() {
    if (isset($this->data['objects'])) {
      $this->saveObjects($this->data['objects'], NULL);
    }

    if (isset($this->data['links'])) {
      $this->saveLinks($this->data['links'], NULL);
    }

    return TRUE;
  }

  /**
   * Save links to the database.
   *
   * @param $links
   *   An array of fetched links from MaPS System®.
   */
  protected function saveLinks(array $links) {
    $links = isset($links['link']) ? $links['link'] : array();

    // Open a transaction to ensure data integrity.
    $transaction = db_transaction();

    // If only one link, force array.
    if (isset($links['link_type'])) {
      $links = array($links);
    }

    try {
      foreach ($links as $link) {
        $query = db_merge(self::DB_LINKS_TABLE)
          ->key(array(
            'pid' => $this->getProfile()->getPid(),
            'source_id' => $link['idobject_from']['value'],
            'target_id' => $link['idobject_to']['value'],
            'type_id' => $link['idlink_type'],
           ))
          ->fields(array(
            'count' => $link['count']['value'],
            'inserted' => time(),
          ))
          ->execute();

        if ($query === SAVED_NEW) {
          $this->incrementCounter('insert', 'link');
        }
        else if ($query === SAVED_UPDATED) {
         $this->incrementCounter('update', 'link');
        }
      }
    }
    catch(\Exception $e) {
      $transaction->rollback();

      if ($e instanceof ImportException) {
        $e->addContext('$profile', $this->getProfile());
        $e->watchdog();
      }
      else {
        watchdog_exception('maps_suite', $e);
      }

      if ($this->isBatch()) {
        $this->context['results']['error'] = $e->getMessage();
        throw $e;
      }
    }

    return TRUE;
  }

  /**
   * @inheritdoc
   */
  protected function fetch(&$context = NULL) {
    // Differential call.
    if (variable_get('maps_import_differential', 0)) {
      if ($min_updated_date = variable_get('maps_import:objects_diff:' . $this->getProfile()->getPid(), FALSE)) {
        $this->arguments['minUpdatedDate'] = $min_updated_date;
      }
    }

    return parent::fetch($context);
  }

  /**
   * @inheritdoc
   */
  protected function updateImportDate() {
    $date = date('Y-m-d H:i:s');

    if (variable_get('maps_import_differential')) {
      variable_set('maps_import:objects_diff:' . $this->getProfile()->getPid(), $date);
    }
    else {
      variable_set('maps_import:objects_full:' . $this->getProfile()->getPid(), $date);
      variable_set('maps_import:objects_diff:' . $this->getProfile()->getPid(), $date);
    }
  }

}
