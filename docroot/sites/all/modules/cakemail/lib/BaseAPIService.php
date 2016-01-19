<?php
/**
 * @file
 */

namespace Drupal\cakemail_api;

/**
 * Default implementation of a CakeMail service. Uses magic method to map
 * inaccessible methods invocation to API calls.
 */
class BaseAPIService {

  /**
   * @var APIInterface
   *   The CakeMailAPIInterface used for methods calls.
   */
  protected $api;

  /**
   * @var string
   *   This service's name.
   */
  protected $name;

  /**
   * Create a new CakeMail API service.
   *
   * The service allow methods invocation through the passed in
   * CakeMailAPIInterface instance.
   *
   * @param string $name
   *   Service name (as documented in CakeMail API, case sensitive).
   * @param APIInterface $api
   *   The CakeMail API instance to use for method calls.
   */
  public function __construct($name, APIInterface $api) {
    $this->name = $name;
    $this->api = $api;
  }

  /**
   * Inaccessible methods invocation are API calls.
   *
   * @param string $name
   *   Method name.
   * @param array $arguments
   *   Method arguments
   *
   * @return mixed
   *   API response.
   */
  public function __call($name, $arguments) {
    return $this->api->call($this->name, $name, $arguments);
  }
}