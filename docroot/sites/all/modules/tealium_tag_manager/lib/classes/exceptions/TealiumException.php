<?php
/**
 * @file
 * Exception class for the Tealium class
 */

class TealiumException extends Exception {
  /**
   * Constructor.
   *
   * @param string $message
   *   Exception Message
   * @param Exception $previous
   *   Previous exception.
   * @param int $code
   *   Exception code.
   */
  public function __construct($message, Exception $previous = NULL, $code = 0) {
    parent::__construct($message, $code, $previous);
  }

  /**
   * Custom string representation of object.
   *
   * @return string
   *   String representation of the Object.
   */
  public function __toString() {
    return __CLASS__ . ": [{$this->code}]: {$this->message}\n";
  }
}
