<?php

/**
 * Class ZuoraResponse
 */
class ZuoraResponse {
  /**
   * @var \stdClass
   */
  protected $raw_response;

  /**
   * @param \stdClass $response
   */
  public function __construct(stdClass $response) {
    $this->raw_response = $response;
  }

  /**
   * @return mixed
   */
  public function data() {
    return drupal_json_decode($this->raw_response->data);
  }

  /**
   * @return bool
   */
  public function successful() {
    return $this->raw_response->code == 200;
  }

  /**
   * @return mixed
   */
  public function error() {
    if (!$this->successful()) {
      return $this->raw_response->error;
    }
  }
}
