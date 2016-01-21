<?php

/**
 * @file
 *
 * SOAP webservice implementation class using PHP's native SOAPClient class
 */

class CollectiveAccessSOAPConnector extends CollectiveAccessConnector {
  /**
   * Execute a method on the remote SOAP service
   */
  public function execute($service, $method, $params = array()) {
    static $client;
    $path = libraries_get_path('nusoap');
    require_once(libraries_get_path('nusoap') . '/lib/nusoap.php');
    if (!array_key_exists('DEBUG', $params) || $params['DEBUG'] != TRUE) {
      nusoap_base::setGlobalDebugLevel(0);
    }
    if (isset($this->collectiveaccess_instance)) {
      $result = NULL;
      try {

        if (!isset($client[$service])) {
          $service_url = $this->collectiveaccess_instance->service_url;
          $service_url .= '/' . drupal_strtolower($service);
          $service_url .= '/' . $service;
          $service_url .= '/soapWSDL';
          $nusoap_client = new nusoap_client($service_url, TRUE, false, false, false, false, 10, 600);
          $nusoap_client->useHTTPPersistentConnection();
          $nusoap_client->soap_defencoding = 'UTF-8';
          $nusoap_client->decode_utf8 = false;
          $client[$service] = $nusoap_client;
        }

        $soapclient = $client[$service];
        $result = $soapclient->call($method, $params);
      } catch (SOAPFault $exception) {
        watchdog('collectiveaccess', 'SOAP ERROR @message', array('@message' => $exception->faultcode . ' ' . $exception->faultstring), WATCHDOG_ERROR);
      }
      return $result;
    }
    else {
      watchdog('collectiveaccess', 'No valid CollectiveAccess instance object available', array(), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  public function get_type() {
    return 'SOAP';
  }
}