<?php
/**
 * @file
 * Defines QueueRunnerException.
 *
 * As per the recommendations at http://drupal.org/node/608166, we are creating
 * an exception specifically for Queue Runner related errors.
 */

/**
 * Exception thrown for Queue runner related errors.
 */
class QueueRunnerException extends Exception {
  public $data = array();

  public function __construct($message, $data = array(), $code = NULL, Exception $previous = NULL) {
    parent::__construct($message, $code, $previous);
    // Make sure we have an array.
    $this->data = (array) $data;
    // Don't log a whole loaded entity.
    unset($this->data['entity']);
  }

  public function __toString() {
    $message = "exception '" . __CLASS__ . "' with message '{$this->message}' in {$this->getFile()}:{$this->getLine()}";
    if ($this->data) {
      $message .= ' | with data:';
    }
    foreach ($this->data as $key => $object) {
      $message .= ' ' . $key . '="' . print_r($object, TRUE) . '"';
    }
    return $message;
  }

  /**
   * Log exception to watchdog.
   *
   * @param string $message
   *   Any extra messages to log.
   */
  public function log() {
    $message = check_plain($this->message);

    $params = array();
    foreach ($this->data as $key => $object) {
      $message .= ' ' . check_plain($key) . '="!' . $key . '"';
      $params['!' . $key] = nl2br(check_plain(print_r($object, TRUE)));
    }

    watchdog(__CLASS__, $message, $params, WATCHDOG_ERROR);
  }
}

