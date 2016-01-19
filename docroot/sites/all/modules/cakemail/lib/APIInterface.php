<?php
/**
 * @file
 */

namespace Drupal\cakemail_api;

/**
 * Entry point to the CakeMAil API.
 *
 * Instance of this interface should provides properties for the services
 * provided by the API. The values for these properties should be
 * CakeMailAPIServiceInterface instances.
 */
interface APIInterface {

  /**
   * Call a CakeMail API Service method.
   *
   * @param string $service
   *   Service name (case sensitive)
   * @param string $method
   *   Method name.
   * @param array $arguments
   *   Method arguments.
   *
   * @return stdClass
   *   Result from the API (abject structure decoded from JSON).
   *
   * @throws APIException
   *   If any error occurs when calling the CakeMail REST API.
   */
  public function call($service, $method, array $arguments);
}
