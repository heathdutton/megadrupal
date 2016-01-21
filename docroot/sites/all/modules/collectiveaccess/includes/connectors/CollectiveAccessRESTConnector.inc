<?php

/**
 * @file
 *
 * REST webservice implementation class for a connector to a CollectiveAccess installation
 */

class CollectiveAccessRESTConnector extends CollectiveAccessConnector {
  // @TODO: implement
  public function execute($service, $method, $params = NULL) {
    return 'executed ' . $api_function;
  }

  public function get_type() {
    return 'REST';
  }
}