<?php
/**
 * @file
 * Contains AnkiServerException class.
 */

/**
 * Exception generated while attempting to communicate with the Anki server.
 */
class AnkiServerException extends Exception {
  /**
   * Construct a new AnkiServerException.
   *
   * @param string $message
   *   Human readable message.
   * @param object $http_response
   *   (Optional) The response object returned by drupal_http_request().
   * @param Exception $previous
   *   (Optional) The previous exception that caused this one.
   *
   * @see drupal_http_request()
   */
  public function __construct($message, $http_response = NULL, Exception $previous = NULL) {
    $this->http_response = $http_response;
    // We don't use 'code', so just pass 0.
    parent::__construct($message, 0, $previous);
  }

  /**
   * Get the HTTP response if there is one.
   *
   * @return object
   *   Returns object from drupal_http_request() or NULL
   *
   * @see drupal_http_request()
   */
  public function getHttpResponse() {
    return $this->http_response;
  }
}
