<?php
/**
 * @file
 * Drupal RestMini Service Test.
 */


/**
 * Class RestMiniServiceTestVar.
 *
 * Used in hook_restmini_service_test() implementation to flag that a value must be retrieved from a previous REST response of current test.
 */
class RestMiniServiceTestVar {

  /**
   * Base route name.
   *
   * @var string
   */
  public $baseRoute;

  /**
   * Service name.
   *
   * @var string
   */
  public $service;

  /**
   * Endpoint name.
   *
   * @var string
   */
  public $endpoint;

  /**
   * Operation name.
   *
   * @var string
   */
  public $operation;

  /**
   * Index of the request against that operation.
   *
   * @var integer
   */
  public $requestIndex;

  /**
   * @var array
   */
  public $keyPath;

  /**
   * @param string $baseRoute
   * @param string $service
   * @param string $endpoint
   * @param string $operation
   * @param integer $requestIndex
   * @param array $keyPath
   *   Key path to value.
   *   Default: empty (~ the whole payload).
   */
  public function __construct($baseRoute, $service, $endpoint, $operation, $requestIndex, $keyPath = array()) {
    $this->baseRoute = $baseRoute;
    $this->service = $service;
    $this->endpoint = $endpoint;
    $this->operation = $operation;
    $this->requestIndex = $requestIndex;
    $this->keyPath = $keyPath ? $keyPath : array();
  }
}
