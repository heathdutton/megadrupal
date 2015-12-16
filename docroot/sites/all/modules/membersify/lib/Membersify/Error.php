<?php

/**
 * Class Membersify_Error
 */
class Membersify_Error extends Exception
{
  /**
   * Constructor method, pass-through to Exception.
   *
   * @param null $message
   * @param null $http_status
   * @param null $http_body
   * @param null $json_body
   */
  public function __construct($message = null, $http_status = null, $http_body = null, $json_body = null)
  {
    parent::__construct($message);
    $this->http_status = $http_status;
    $this->http_body = $http_body;
    $this->json_body = $json_body;
  }

  /**
   * Returns the HTTP Status of the error.
   *
   * @return string
   */
  public function getHttpStatus()
  {
    return $this->http_status;
  }

  /**
   * Returns the HTTP Body of the error.
   *
   * @return mixed
   */
  public function getHttpBody()
  {
    return $this->http_body;
  }

  /**
   * Returns the JSON body of the error.
   *
   * @return mixed
   */
  public function getJsonBody()
  {
    return $this->json_body;
  }
}
