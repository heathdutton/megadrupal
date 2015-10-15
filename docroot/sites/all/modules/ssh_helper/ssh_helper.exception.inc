<?php

/**
 * @file
 * Contains SshHelperException.
 *
 * As per the recommendations at http://drupal.org/node/608166, we are creating
 * an exception specifically for SSH helper related errors.
 */

/**
 * Exception thrown for SSH helper related errors.
 */
class SshHelperException extends Exception {
  public $data = array();

  public function __construct($message, $data = NULL, $code = NULL) {
    parent::__construct($message, $code);

    $this->data = $data;
  }

  /**
   * Log exception to watchdog
   *
   * @param string $message
   *   Optionally can specify extra own message
   */
  public function log($message = NULL, $entity_id = NULL, $entity_type = NULL) {
    // Make the error message a string
    $message = implode('; ', array_filter(array($message, $this->getMessage())));

    // Add
    $params = array();
    if (isset($this->data)) {
      foreach ($this->data as $key => $object) {
        $message .= ' ' . check_plain($key) . ' <pre>@' . $key . '</pre>';
        $params['@' . $key] = $object;
      }
    }
    watchdog(get_class($this), $message, $params, WATCHDOG_ERROR);
  }
}
