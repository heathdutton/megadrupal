<?php

/**
 * @file
 * Custom exception definitions
 */

class ServicesClientConnectionException extends Exception {

  /**
   * Save exception to log
   *
   * @param $severity
   *   Watchdog severity default set to ERROR
   */
  public function log($severity = WATCHDOG_ERROR) {
    watchdog('scc', check_plain($this->getMessage()), array(), $severity);
  }
}

/**
 * Common parent class for all services client module exceptions
 */

class ServicesClientConnectionResponseException extends Exception {

  /**
   * Whole response that represents failed request
   *
   * @var ServicesClientConnectionResponse
   */
  public $response;

  /**
   * Request that has been made to server
   *
   * @var ServicesClientConnectionHttpRequest
   */
  public $request;

  public function __construct($response, $request, $message = NULL, $code = NULL) {
    // Save request and response source
    $this->response = $response;
    $this->request = $request;
    $message = empty($message) ? $this->getServicesMessage() : $message;
    parent::__construct($message, $code);
  }

  /**
   * Dump request and response to watchdog
   *
   * @param $severity
   *   Watchdog severity default set to ERROR
   */
  public function log($severity = WATCHDOG_ERROR) {
    watchdog('scc', 'Request: <pre>@request</pre> Error response: <pre>@response</pre>', array(
      '@request' => print_r($this->request, TRUE),
      '@response' => print_r($this->response, TRUE),
    ), $severity);
  }

  /**
   * Provide human readable error mesage.
   */
  public function getServicesMessage() {
    $msg = $this->response->error_code . ' ' . $this->response->error_message;
    if (!empty($this->response->services_error)) {
      $msg .= ': ' . $this->response->services_error;
    }
    return $msg;
  }

  /**
   * Return error reponse code.
   *
   * @return int
   *   HTTP or cURL error code.
   */
  public function getErrorCode() {
    return $this->response->error_code;
  }

  /**
   * Return error response message.
   *
   * @return string
   *   Description message.
   */
  public function getErrorMessage() {
    return $this->response->error_message;
  }
}
