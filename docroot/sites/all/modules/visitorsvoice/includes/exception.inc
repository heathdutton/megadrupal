<?php

/**
 * @file
 * Contains VisitorsvoiceException.
 */

/**
 * Represents an exception that occurred in this module.
 *
 * The code set on this exception represents the HTTP status code that should be
 * used for this page request when it is terminated by this exception.
 */
class VisitorsvoiceException extends Exception {

  /**
   * Terminate the current page request with information from this exception.
   */
  public function terminateRequest() {
    header($_SERVER['SERVER_PROTOCOL'] . ' ' . $this->getHttpStatus());
    echo $this->getMessage();
    echo "\n";
    echo $this->getTraceAsString();
  }

  /**
   * Get a correct HTTP status code/message for the code set on this exception.
   *
   * @return string
   *   The HTTP status code/message to use.
   */
  protected function getHttpStatus() {
    $responses = array(
      403 => 'Forbidden',
      404 => 'Not Found',
      500 => 'Internal Server Error',
    );
    $code = (int) $this->getCode();
    if (!isset($responses[$code])) {
      $code = 500;
    }
    return $code . ' ' . $responses[$code];
  }

}
