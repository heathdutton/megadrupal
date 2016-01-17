<?php

/**
 * @file
 * response object returned by tumblr.
 */

/**
 * Class implememtation of tumblr web service response message.
 */
class TumblrResponse {


  /**
   * Http response code returned by the tubmlr remote service.
   *
   * @var string.
   */
  protected $http_code;

  /**
   * Response description string returned by the tubmlr remote service.
   *
   * @var string.
   *   Contains the remote tumblr service post-id identifier during a sucessful response.
   *
   */
  protected $data;

  /**
   * Error description string returned by the tubmlr remote service.
   *
   * @var string.
   */
  protected $error;

  /**
   * Status message returned by the tubmlr remote service.
   *
   * @var string.
   */
  protected $status_message;

  /**
   * Tubmlr remote service internal content type.
   *
   * @var string.
   */
  protected $usec;

  /**
   * Constructs a TumblrResponse object.
   *
   * @param $obj
   *   raw http response data.
   */
  function __construct($obj) {
    $this->http_code = $obj->code;
    if (isset($obj->data)) {
      $this->data = $obj->data;
    }
    if (isset($obj->error)) {
      $this->error = $obj->error;
    }
    if (isset($obj->status_message)) {
      $this->status_message = $obj->status_message;
    }
    if (isset($obj->headers['x-tumblr-usec'])) {
      $this->usec = $obj->headers['x-tumblr-usec'];
    }
  }

  /**
   * Populates TumblrResponse properties.
   *
   * @param $obj
   *   raw http response data
   */
  public function init($obj) {
    if (isset($obj->http_code)) {
      $this->http_code = $obj->http_code;
    }

    if (isset($obj->data)) {
      $this->data = $obj->data;
    }
    if (isset($obj->error)) {
      $this->error = $obj->error;
    }
    if (isset($obj->status_message)) {
      $this->status_message = $obj->status_message;
    }
    if (isset($obj->headers['x-tumblr-usec'])) {
      $this->usec = $obj->headers['x-tumblr-usec'];
    }
  }

  public function getHttpCode() {
    return $this->http_code;
  }

  public function setHttpCode($http_code) {
    $this->http_code = $http_code;
  }

  public function getData() {
    return $this->data;
  }

  public function setData($data) {
    $this->data = $data;
  }

  public function getError() {
    return $this->error;
  }

  public function setError($error) {
    $this->error = $error;
  }

  public function getStatusMessage() {
    return $this->status_message;
  }

  public function setStatusMessage($status_message) {
    $this->status_message = $status_message;
  }

  public function getUsec() {
    return $this->usec;
  }

  public function setUsec($usec) {
    $this->usec = $usec;
  }

  public function toString() {
    return t('Status: @statuse_message  Data: @data', array('@status_message' => $this->status_message, '@data' => $this->data));
  }

}
