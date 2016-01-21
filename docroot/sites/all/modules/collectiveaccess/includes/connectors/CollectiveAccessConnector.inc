<?php

/**
 * @file
 *
 * Generic class for a connector to a CollectiveAccess installation
 */

/**
 * @TODO: add better error handling
 */

abstract class CollectiveAccessConnector {
  protected $collectiveaccess_instance;

  public function __construct(CollectiveAccessInstance $instance) {
    $this->collectiveaccess_instance = $instance;
  }

  /**
   * Execute an API function on the CollectiveAccess API
   */
  abstract function execute($service, $method, $params = NULL);

  abstract function get_type();
}