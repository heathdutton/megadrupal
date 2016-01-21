<?php

/**
 * @file
 * Definition of WoW\HttpResponse.
 */

/**
 * Defines the WoW\HttpResponse abstract class.
 */
abstract class WoWHttpResponse {

  /**
   * An integer containing the response status code, or the error code if an
   * error occurred.
   *
   * @var int
   */
  protected $code;

  /**
   * The response protocol (e.g. HTTP/1.1 or HTTP/1.0).
   *
   * @param string
   */
  protected $protocol;

  /**
   * A string containing the request body that was sent.
   *
   * @var string
   */
  protected $request;

  /**
   * An array containing the response headers as name/value pairs. HTTP header
   * names are case-insensitive (RFC 2616, section 4.2), so for easy access the
   * array keys are returned in lower case.
   *
   * @var array
   */
  protected $headers;

  /**
   * The date of the request used to sign authentication.
   *
   * @var DateTime
   */
  protected $date;

  /**
   * The data returned by the service.
   *
   * @var string
   */
  protected $data;

  /**
   * The data as array. This is primarily used by functions to create entities.
   *
   * @var array
   */
  protected $dataArray;

  /**
   * The data as object. This is used to get the original message structure.
   *
   * @var object
   */
  protected $dataObject;

  public function __construct($response, $date) {
    $this->code = $response->code;
    $this->protocol = $response->protocol;
    $this->request = $response->request;
    $this->headers = $response->headers;

    $this->data = $response->data;
    $this->date = $date;
  }

  /**
   * @return int
   *   An integer containing the response status code, or the error code if an
   *   error occurred.
   */
  public function getCode() {
    return $this->code;
  }

  /**
   * @return string
   *   The response protocol (e.g. HTTP/1.1 or HTTP/1.0).
   */
  public function getProtocol() {
    return $this->protocol;
  }

  /**
   * Return request.
   *
   * @var string
   */
  public function getRequest() {
    return $this->request;
  }

  /**
   * Return headers.
   *
   * @var array
   */
  public function getHeaders() {
    return $this->headers;
  }

  /**
   * Get the data.
   *
   * @param string $key
   *   (Optionnal) A key to return.
   *
   * @return array|mixed
   *   The data returned by the service as an array or a value if a key was given.
   */
  public function getArray($key = NULL) {
    if (empty($this->dataArray)) {
      $this->dataArray = drupal_json_decode($this->data);
    }
    return isset($key) && isset($this->dataArray[$key]) ? $this->dataArray[$key] : $this->dataArray;
  }

  /**
   * Get the data.
   *
   * @param string $key
   *   (Optionnal) A key to return.
   *
   * @return object|mixed
   *   The data returned by the service as an object or a value if a key was given.
   */
  public function getObject($key = NULL) {
    if (empty($this->dataObject)) {
      $this->dataObject = json_decode($this->data);
    }
    return isset($key) && isset($this->dataObject->{$key}) ? $this->dataObject->{$key} : $this->dataObject;
  }

  /**
   * Get the date.
   *
   * @return DateTime
   *   The date with which the request was made.
   */
  public function getDate() {
    return $this->date;
  }
}
