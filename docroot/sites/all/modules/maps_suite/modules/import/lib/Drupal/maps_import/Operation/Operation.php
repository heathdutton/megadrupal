<?php

/**
 * @file
 * Define the MaPS Import Operation base class.
 */

namespace Drupal\maps_import\Operation;

use Drupal\maps_suite\Log\Logger;
use Drupal\maps_suite\Log\Observer\Watchdog as WatchdogObserver;
use Drupal\maps_import\Log\Observer\Mail as MailObserver;
use Drupal\maps_import\Exception\OperationException;
use Drupal\maps_import\Profile\Profile;

/**
 * MaPS Import Operation class.
 */
abstract class Operation implements OperationInterface {

  /**
   * The related maPS Import Profile instance.
   *
   * @var Profile
   */
  protected $profile;

  /**
   * Store the operation context.
   *
   * @var array
   */
  protected $context;

  /**
   * Return the related profile
   *
   * @return Profile
   *   The related profile.
   */
  public function getProfile() {
    return $this->profile;
  }

  /**
   * @inheritdoc
   */
  public function getFullDescription() {
    $output = '<h2>' . $this->getTitle() . '</h2>';
    $description = $this->getDescription();

    if (!empty($description)) {
      $output .= '<p>' . $description . '</p>';
    }

    return $output;
  }

  /**
   * @inheritdoc
   */
  public function isBatch() {
    return isset($this->context['results']['batch_id']);
  }

  /**
   * Detect if the batch is launch by Drush or not.
   */
  public function isDrush() {
    return drupal_is_cli();
  }

  /**
   * @inheritdoc
   */
  public function batchOperations() {
    return array(
      array(
        // Callback function
        array($this, 'process'),
        // Callback arguments
        array(),
      ),
    );
  }


  /**
   * @inheritdoc
   */
  public function getTotalOperations(array $args = array()) {
    return 1;
  }

  /**
   * Perform the operation the class is created for.
   *
   * When the operation is run through the batch API, the last argument
   * is the batch context.
   */
  final public function process() {
    $args = func_get_args();

    if ($batch = &batch_get()) {
      $count = func_num_args();
      array_pop($args);

      // func_get_args() does not contain parameters references but a copy
      // of them (except for objects since PHP 5.3).
      $stack = debug_backtrace();
      $context = & $stack[0]['args'][($count - 1)];

      $context['results']['batch_id'] = $batch['id'];

      if (!isset($context['results']['logs'])) {
        $context['results']['logs'] = array();
      }
    }
    else {
      $context = array('results' => array('batch_id' => NULL));
    }

    $log = Logger::attachLog($this->getType(), $context['results']['batch_id'])->setIntroductionMessage($this->getTitle())->addObserver(new WatchdogObserver());
    $log->setRelatedToken('maps_import_profile', $this->getProfile());

    if (!isset($context['results'][$this->getType()])) {
      $context['results'][$this->getType()] = array(
        'messages' => array(),
        'processed_op' => 0,
        'class' => get_class($this),
        'operation' => $this->getType(),
      );
    }

    $context['results']['logs'][$this->getType()] = (string) $log;

    $this->context = &$context;
    $this->context['results'][$this->getType()]['processed_op']++;

    if ((!$this->isDrush() && variable_get('maps_import_log_manual', 0)) ||
      ($this->isDrush() && variable_get('maps_import_log_drush', 0) && !variable_get('maps_import_log_drush_global_notifications', 0))) {
        $log->addObserver(new MailObserver());
    }

    // Let the class do its job.
    $this->context['results']['process'] = call_user_func_array(array($this, 'processRun'), $args);

    // We experienced some issues with the context variable reference.
    // So we force the batch current set results with our context results.
    if ($this->isDrush()) {
      $current_set =& _batch_current_set();
      $current_set['results'] = $this->context['results'];
    }

    $this->logMemoryPeak();
    $this->getLog()->save();
  }

  /**
   * Extending classes use this method to perform an operation.
   *
   * @return bool
   *   Whether the operation scceeded or not.
   */
  abstract protected function processRun($index = 1);

  /**
   * @inheritdoc
   */
  public function getlog() {
    if (isset($this->context)) {
      return Logger::getLog($this->getType());
    }

    throw new OperationException('MaPS Import operation %operation: try to get the log object while the context has not been instanciated.', 0, array('%operation' => $this->getType()));
  }

  /**
   * Log the memory usage if necessary.
   *
   * @todo move this method to the Log class directly?
   */
  protected function logMemoryPeak() {
    $memory_peak = memory_get_peak_usage();
    $messages = $this->getlog()->moveToContentRoot()->goToContext('memory_peak', TRUE)->getMessages();

    if (empty($messages) || (int) $memory_peak > (int) reset($messages)) {
      $this->getlog()->addMessage(format_size($memory_peak), array(
        'autocreate' => TRUE,
        'delete_messages' => TRUE,
      ));
    }
  }

}
