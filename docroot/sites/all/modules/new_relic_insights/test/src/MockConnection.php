<?php

/**
 * @file
 * Contains a mock Client connection for use in unit tests.
 */


class Connection {

  /**
   * The callMethodArray type for the most recent connection call.
   * @var string
   */
  public $callMethodType = '';

  /**
   * The callMethodArray options for the most recent connection call.
   * @var array
   */
  public $callMethodOptions = array();

  /**
   * The data with which callMethodArray should respond.
   * @var array
   */
  public $callMethodResponseResults = array();

  public function __construct(array $callMethodArrayResponseResults) {
    $this->callMethodResponseResults = $callMethodArrayResponseResults;
  }

  public function callMethodArray($type, $options) {
    $this->callMethodType = $type;
    $this->callMethodOptions = $options;

    $response = new stdClass();
    $response->results = $this->callMethodResponseResults;

    return $response;
  }

}
