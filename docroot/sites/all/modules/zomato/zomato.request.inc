<?php

/**
 * @file
 * API for making call to Zomato API.
 */

/**
 * Request object for Zomato API.
 */
class ZomatoApi {
  private $key;
  private $base_url;

  function __construct() {
    $this->key = variable_get('zomato_api_key', '');
    $this->base_url = variable_get('zomato_base_url', '');
  }

  /**
   * Performs an API request.
   *
   * @param $call
   *  name of the API call being made.
   * @param $options
   *  (optional) an array taking the following keys:
   *  - params: (optional) an associative array of parameters to be sent along with the call. The key should be the name of the parameter and the value of that key should be the value to be passed for that parameter.
   *  - http_request_options: (optional) takes $options parameter of drupal_http_request(). For example, making a GET or POST call, you will have to set the 'method' key to GET or POST just like one would do it while using drupal_http_request().
   * @return same as response returned by drupal_http_request().
   */
  function request($call, $options = array()) {
    $params = array();
    if (isset($options['params'])) {
      foreach ($options['params'] as $param => $value) {
        $params[$param] = $value;
      }
    }
    $query = http_build_query($params);

    $options['http_request_options']['headers']['X-Zomato-API-Key'] = $this->key;

    $response = drupal_http_request($this->base_url . $call . '?' . $query, $options['http_request_options']);

    return $response;
  }
}
