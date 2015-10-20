<?php

/**
 * @file
 * Handles import functionality.
 */

namespace Drupal\maps_import\Fetcher;

use Drupal\maps_import\Cache\Object\Profile as CacheProfile;
use Drupal\maps_import\Exception\Exception as ImportException;
use Drupal\maps_import\Operation\Operation;
use Drupal\maps_import\Profile\Profile;
use Drupal\maps_import\Request\Request;
use Drupal\maps_suite\Log\Logger;
use Drupal\maps_suite\Log\Context\Context as LogContext;

/**
 * Base class for all fetch operations.
 *
 * Implements Serializable for reduce the size of data in batch operations.
 */
abstract class Fetcher extends Operation implements \Serializable {

  /**
   * Base timeout for HTTP request.
   *
   * @todo remove that and use a separate process that calls a given URL
   * when the data are fully fetched.
   */
  const TIMEOUT = 240; // 4 minutes

  /**
   * Template export web.
   */
  const TEMPLATE_EXPORT_WEB = 'export_web';

  /**
   * Template export web.
   */
  const TEMPLATE_EXPORT_WEB_DEMO = 'export_web_demo';

  /**
   * Fetched objects database table.
   */
  const DB_OBJ_TABLE = 'maps_import_objects';

  /**
   * Fetched links database table.
   */
  const DB_LINKS_TABLE = 'maps_import_links';

  /**
   * Fetched configuration database table.
   */
  const DB_CONF_TABLE = 'maps_import_configuration';

  /**
   * Database table for library items.
   */
  const DB_LIBRARY_TABLE = 'maps_import_libraries';

  /**
   * Database table for medias.
   */
  const DB_MEDIA_TABLE = 'maps_import_medias';

  /**
   * Database table for objects and medias mapping
   */
  const DB_OBJECT_MEDIA_TABLE = 'maps_import_object_media';

  /**
   * The Web Services extra arguments.
   *
   * @var array
   * @see Request::getData()
   */
  protected $arguments = array();

  /**
   * Extra options for Drupal HTTP request.
   *
   * @var array
   * @see Request::getData()
   */
  protected $options = array();

  /**
   * The parsed data from Web Services response.
   *
   * @var array
   * @see Request::getData()
   */
  protected $data;

  /**
   * Class constructor.
   *
   * @param $profile
   *   The profile instance.
   *
   * @return Fetcher
   */
  public function __construct(Profile $profile) {
    $this->profile = $profile;
    $this->setArguments(array('templates' =>  variable_get('maps_import_log_template', $profile->getWebTemplate())));
  }

  /**
   * Set the timeout for an HTTP request and adjust the PHP
   * time limit as well.
   *
   * Caution: calling this method from a normal page may result in
   * a blank page, due to the browser time out. So it should only
   * be used fot batch operations (CRON, etc.).
   *
   * @param $timeout
   *    The timeout to set.
   */
  protected function setTimeout($timeout) {
    static $max_execution_time;

    if (!isset($max_execution_time)) {
      $max_execution_time = ini_get('max_execution_time');
    }

    // Do not modify the maximum execution time  when running PHP in command
    // line (time forced to 0) or if the maximum execution time is zero.
    if ($this->isDrush() || 0 == $max_execution_time) {
      return;
    }

    $this->options['timeout'] = $timeout;
    set_time_limit($timeout + $max_execution_time);
  }

  /**
   * Sets arguments for the Web Services request.
   *
   * @see Request::getData()
   */
  public function setArguments(array $arguments, $replace = FALSE) {
    if ($replace) {
      $this->arguments = $arguments;
    }
    else {
      $this->arguments = array_merge($this->arguments, $arguments);
    }
  }

  /**
   * Sets options for Drupal HTTP request.
   *
   * @see Request::getData()
   */
  public function setOptions(array $options, $replace = FALSE) {
    if ($replace) {
      $this->options = $options;
    }
    else {
      $this->options = array_merge($this->options, $options);
    }
  }

  /**
   * Fetches then saves the data.
   */
  protected function processRun($index = 0, $range = 0, $last = TRUE) {
    if (empty($this->context['message'])) {
      // Message displayed under the progressbar.
      $this->context['message'] = t('Fetching data for profile "@title"', array('@title' => $this->getProfile()->getTitle()));
    }

    if ($this->fetch()) {
      return $this->save();
    }

    return FALSE;
  }

  /**
   * Fetches data.
   *
   * @return boolean
   *   True on success, FALSE on failure.
   */
  protected function fetch() {
    if (FALSE !== $this->data = Request::getData($this->getProfile(), $this->arguments, $this->options)) {
      if (!isset($this->context['results']['indexes']['fetch'])) {
        $this->context['results']['indexes']['fetch'] = 1;
      }
      else {
        $this->context['results']['indexes']['fetch']++;
      }

      $this->getLog()->addMessage($this->context['results']['indexes']['fetch'], array(
        'root' => TRUE,
        'context' => 'fetch',
        'delete_messages' => TRUE,
      ));
    }

    return (FALSE !== $this->data);
  }

  protected abstract function save();

  /**
   * Update the import date for the current fetching.
   */
  protected abstract function updateImportDate();

  /**
   * Save objects to the database.
   *
   * @param $objects
   *   An array of fetched objects from MaPS System®.
   * @param $config_type
   *   If specified, this indicates the type of configuration object, for
   *   example "linked" or "attribute".
   */
  protected function saveObjects(array $objects, $config_type = NULL) {
    // Open a transaction to ensure data integrity.
    $transaction = db_transaction();

    try {
      $existing = db_select(self::DB_OBJ_TABLE)
        ->fields(self::DB_OBJ_TABLE, array('id', 'inserted'))
        ->condition('pid', $this->getProfile()->getPid())
        ->execute()
        ->fetchAllKeyed();

      foreach ($objects as $object) {
        $id = $object['idobject']['value'];

        $fields = array(
          'pid' => $this->getProfile()->getPid(),
          'id' => $id,
          'parent_id' => $object['idobject_parent']['value'],
          'weight' => $object['seq']['value'],
          'source_id' => $object['idobject_origin']['value'],
          'status' => $object['status']['value'],
          'code' => $object['code']['value'],
          'nature' => $object['nature']['value'],
          'type' => $object['object_type']['value'],
          'updated' => strtotime($object['lastmodification']['value']),
          'inserted' => time(),
          'config_type' => $config_type,
        );

        // Add classes to the fields
        $classes = isset($object['classes']['idclass']) ? $object['classes']['idclass'] : array();

        // Sorting the classe IDs is important when dealing with dynamic
        // bundles, while creating the bundle name.
        sort($classes);
        $fields['classes'] = serialize($classes);

        // Simplify attributes: we keep only what is necessary
        $fields['attributes'] = serialize($this->reduceMultipleAttributes($object['attributes']));

        // Delete all associations with media for this object
        db_delete(self::DB_OBJECT_MEDIA_TABLE)
          ->condition('object_id', $id)
          ->condition('pid', $this->getProfile()->getPid())
          ->execute();

        $this->saveMedias($object['medias'], $id);

        if (isset($existing[$fields['id']])) {
          $type = 'update';
          $query = db_update(self::DB_OBJ_TABLE)
            ->condition('id', $fields['id'])
            ->condition('pid', $this->getProfile()->getPid());

          // If the current object was previously inserted into database
          // but not processed, we have to keep its process order to avoid
          // integrity issues.
          if (!empty($existing[$fields['id']])) {
            unset($fields['inserted']);
          }
        }
        else {
          $type = 'insert';
          $query = db_insert(self::DB_OBJ_TABLE);
        }

        $query
          ->fields($fields)
          ->execute();

        if (is_null($config_type)) {
          $log_config_type = 'object';
        }
        else {
        	$log_config_type = $config_type;
        }

        if (method_exists($this, 'incrementCounter')) {
          $this->incrementCounter($type, $log_config_type);
        }
      }

    }
    catch(Exception $e) {
      $transaction->rollback();

      if ($e instanceof ImportException) {
        $e->addContext('$profile', $this->getProfile());
        $e->watchdog();
      }
      else {
        watchdog_exception('maps_suite', $e);
      }

      $this->context['results']['error'] = $e->getMessage();
      throw $e;
    }

    return TRUE;
  }

  /**
   * Save object into database.
   *
   * @param $medias
   *   The array of media to save.
   *
   * @param $id_object
   *   The object id medias are linked to.
   *
   * @return boolean
   */
  protected function saveMedias(array $medias, $id_object) {
    // Open a transaction to ensure data integrity.
    $transaction = db_transaction();

    try {
      if (empty($medias)) {
        return TRUE;
      }

      foreach ($medias as $id_media => $media) {
        $query = FALSE;

        // Check if entry with this id already exists
        $existing = db_select(self::DB_MEDIA_TABLE)
          ->fields(self::DB_MEDIA_TABLE, array('id', 'filename'))
          ->condition('pid', $this->getProfile()->getPid())
          ->execute()
          ->fetchAllKeyed();

        $fields = array(
          'id' => $id_media,
          'pid' => $this->getProfile()->getPid(),
          'extension' => $media['extension'],
          'type' => $media['type'],
          'url' => $media['url'],
          'weight' => $media['weight'],
          'filename' => $media['filename']['value'],
          'attributes' => serialize($this->reduceMultipleAttributes($media['attributes'])),
          'inserted' => time(),
          'updated' => isset($media['updated']) ? strtotime($media['updated']) : time(),
        );

        if (!isset($existing[$id_media])) {
          $this->context['medias'][$id_media] = TRUE;

          $query = db_insert(self::DB_MEDIA_TABLE);

          if (method_exists($this, 'incrementCounter')) {
            $this->incrementCounter('insert', 'media');
          }
        }
        else {
          if (!isset($this->context['medias'][$id_media])) {
            $query = db_update(self::DB_MEDIA_TABLE)
              ->condition('id', $fields['id'])
              ->condition('pid', $this->getProfile()->getPid());

            if (method_exists($this, 'incrementCounter')) {
              $this->incrementCounter('update', 'media');
            }
          }
        }

        if ($query) {
          $query
            ->fields($fields)
            ->execute();

          // Add the association object/media in database
          db_insert(self::DB_OBJECT_MEDIA_TABLE)
            ->fields(array(
              'object_id' => $id_object,
              'media_id' => $id_media,
              'pid' => $this->getProfile()->getPid(),
            ))
            ->execute();
        }
      }
    }
    catch (\Exception $e) {
      $transaction->rollback();
      $this->getLog()->addMessageFromException($e);

      $this->context['results']['error'] = $e->getMessage();
      throw $e;
    }
  }

  /**
   * Symplify an aray of attributes.
   */
  protected function reduceMultipleAttributes(array $attributes) {
    $return = array();
    foreach ($attributes as $id=> $attribute) {
      if (empty($attribute['values'])) {
        continue;
      }

      foreach ($attribute['values'] as $idLanguage => $values) {
        $return[$id][$idLanguage] = !empty($values) && is_array($values) ? maps_suite_reduce_array($values, 'value') : NULL;
      }
    }

    return array_filter($return);
  }


  /**
   * Increase the number of inserted/updated objects.
   */
  public function incrementCounter($type, $element = 'object') {
    // Create element node in log
    if (!isset($this->context['results']['indexes'][$element])) {
      Logger::getLog($this->getType())
        ->moveToContentRoot()
        ->goToContext('count', TRUE)
        ->addContext(new LogContext($element), 'child');
    }

    // Initialize type
    if (!isset($this->context['results']['indexes'][$element][$type])) {
      if (in_array($type, array('insert', 'update'))) {
        $this->context['results']['indexes'][$element][$type] = 1;

        Logger::getLog($this->getType())
          ->moveToContentRoot()
          ->goToContext('count')
          ->goToContext($element)
          ->addContext(new LogContext($type), 'child')
          ->addMessage(1);
      }
    }
    // Update
    else {
      if (in_array($type, array('insert', 'update'))) {
        $this->context['results']['indexes'][$element][$type]++;

        Logger::getLog($this->getType())
          ->moveToContentRoot()
          ->goToContext('count')
          ->goToContext($element)
          ->goToContext($type)
          ->deleteMessages()
          ->addMessage((int) $this->context['results']['indexes'][$element][$type]);
      }
    }

    $this->incrementTotalCounter();
  }

  /**
   * Increase the number of total operation done.
   */
  public function incrementTotalCounter() {
    if (!isset($this->context['results']['total'])) {
      $this->context['results']['total'] = 1;

      Logger::getLog($this->getType())
        ->moveToContentRoot()
        ->addContext(new LogContext('total'), 'child')
        ->addMessage(1);
    }
    else {
      $this->context['results']['total']++;

      Logger::getLog($this->getType())
        ->moveToContentRoot()
        ->goToContext('total', TRUE)
        ->deleteMessages()
        ->addMessage((int) $this->context['results']['total']);
    }
  }

  /**
   * @inheritdoc
   * @see \Serialize
   */
	public function serialize() {
	  return serialize(array(
	    'pid' => $this->profile->getPid(),
	    'arguments' => $this->arguments,
	    'options' => $this->options,
	  ));
	}

  /**
   * @inheritdoc
   * @see \Serialize
   */
	public function unserialize($data) {
		$data = unserialize($data);
    $this->profile = CacheProfile::getInstance()->loadSingle($data['pid']);
    $this->arguments = $data['arguments'];
    $this->options = $data['options'];
  }

}
