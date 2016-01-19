<?php
/**
 * @file
 */

namespace Drupal\cakemail_api;

/**
 * Implementation of CakeMail APIInterface based on drupal_http_request().
 *
 * The class is configured on instantiation using the following variables:
 *  - cakemail_api_key: Key to use for all API calls.
 *  - cakemail_api_url: Base URL for the API REST endpoint.
 */
class DrupalHttpRequestAPI implements APIInterface {
  /**
   * @var string
   *   URL of the REST endpoint for the API.
   */
  protected $api_url;

  /**
   * @var string
   *   API key.
   */
  protected $api_key;

  /**
   * @var array
   *   Service instances. Every entry should be a BaseCakeMailServiceApi instance.
   */
  protected $services = array();

  /**
   * Create a new instance of the API interface.
   */
  public function __construct() {
    $this->api_key = variable_get('cakemail_api_key', NULL);
    $this->api_url = variable_get('cakemail_api_url', CAKEMAIL_API_DEFAULT_ENDPOINT);
  }

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
  public function call($service, $method, array $arguments) {
    if (variable_get('cakemail_cache_enabled', TRUE) && $this->isCacheEnabled($service, $method)) {
      $cid_parts = array('cakemail', $service, $method, hash('md5', print_r(ksort($arguments), TRUE)));
      $cid = implode(':', $cid_parts);
      $cache = cache_get($cid);
      if ($cache && $cache->created > (REQUEST_TIME - (variable_get('cakemail_cache_maxage', CAKEMAIL_API_DEFAULT_CACHE_MAXAGE)))) {
        return $cache->data;
      }
    }
    else {
      $cid = NULL;
    }


    $url = "{$this->api_url}/$service/$method";
    $headers = array(
      'apikey' => $this->api_key,
      'Content-Type' => 'application/x-www-form-urlencoded',
    );

    $context = stream_context_create(array(
      'ssl' => array(
        'verify_peer' => variable_get('cakemail_api_ssl_verify_peer', FALSE),
      ),
    ));

    $options = array(
      'headers' => $headers,
      'method' => 'POST',
      'data' => drupal_http_build_query($arguments),
      'context' => $context,
    );
    $response = drupal_http_request($url, $options);

    if (($debug_callback = variable_get('cakemail_api_debug_callback', FALSE)) && function_exists($debug_callback)) {
      $debug_callback($url, $options, $response);
    }

    try {
      if (!empty($response->error)) {
        throw new APIException('HTTP Request Error: ' . $response->error, $service, $method, $arguments, $response->code);
      }
      else {
        // Decode JSON encoded answer.
        $result = json_decode($response->data);
        // Manage JSON decoding error.
        $json_error = json_last_error();
        if ($json_error != JSON_ERROR_NONE) {
          $error_message = 'Unable to decode JSON';
          switch ($json_error) {
            case JSON_ERROR_DEPTH:
              $error_message .= ': The maximum stack depth has been exceeded.';
              break;

            case JSON_ERROR_STATE_MISMATCH:
              $error_message .= ': Invalid or malformed JSON.';
              break;

            case JSON_ERROR_CTRL_CHAR:
              $error_message .= ': Control character error, possibly incorrectly encoded.';
              break;

            case JSON_ERROR_SYNTAX:
              $error_message .= ': Syntax error.';
              break;

            case JSON_ERROR_UTF8:
              $error_message .= ': Malformed UTF-8 characters, possibly incorrectly encoded.';
              break;

            default:
              $error_message .= '.';
          }
          throw new APIException($error_message, $service, $method, $arguments, $json_error);
        }
        elseif ($result->status != 'success') {
          throw new APIException($result->data, $service, $method, (array) $result->post);
        }
        else {
          if ($cid) {
            cache_set($cid, $result->data, 'cache', CACHE_TEMPORARY);
          }
          return $result->data;
        }
      }
    }
    catch (APIException $e) {
      // Log the error using Drupal's watchdog.
      watchdog('cakemail', "CakeMail API Exception: %message\n<pre>\n@stacktrace</pre>", array(
        '%message' => $e->getMessage(),
        '@stacktrace' => $e->getTraceAsString(),
      ), WATCHDOG_ERROR);
      // Re-throw the Exception to let caller handle it.
      throw $e;
    }

  }

  /**
   * Map inaccessible properties to BaseCakeMailServiceApi instances.
   */
  public function __get($name) {
    $name = drupal_ucfirst($name);
    if (!isset($this->services[$name])) {
      $this->services[$name] = $this->createService($name);
    }
    return $this->services[$name];
  }

  /**
   * Create a new service instance for the given service name.
   *
   * @param string $name
   *   A service name (case sensitive).
   *
   * @return BaseAPIService
   *   A service instance for the given name.
   */
  protected function createService($name) {
    switch ($name) {
      // Add known service name to class map here.
      default:
        return new BaseAPIService($name, $this);
    }
  }

  /**
   * Returns whether or not cache is enabled for given service method.
   *
   * @param string $service
   *   A service name (case sensitive)
   * @param string $method
   *   A service method name (case sensitive)
   *
   * @return bool
   *   TRUE if the result from a call to the given service method can be cached.
   */
  protected function isCacheEnabled($service, $method) {
    // Cache is enabled for all Get* methods.
    return drupal_substr($method, 0, 3) == 'Get';
  }
}