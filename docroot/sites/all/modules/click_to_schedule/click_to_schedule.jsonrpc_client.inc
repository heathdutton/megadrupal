<?php

/**
 * @file
 * Class for a simple JSON-RPC client.
 */

/**
 * Implements a simple JSON-RPC client.
 *
 * Adapted from the jsonrpc_client module to work with the TimeTrade JSON-RPC
 * server. This version can receive and send cookies.
 */
class JsonRpcClient {

  /**
   * The URL of the JSON-RPC service.
   */
  protected $url;

  /**
   * The JSON version.
   */
  protected $version;

  /**
   * An API key for authentication.
   */
  protected $apikey;

  /**
   * Domain for API key authentication.
   */
  protected $domain;

  /**
   * The request/response ID.
   */
  protected $id;

  /**
   * The debug message type.
   */
  protected $debug = NULL;

  /**
   * Whether to check that the response ID matches the request ID.
   */
  protected $checkId = TRUE;

  /**
   * Constructs a JsonRpcClient object.
   *
   * @param string $url
   *   URL of the service, e.g. "http://example.com/services/json-rpc".
   * @param string $version
   *   JSON version (Default 2.0).
   * @param string $apikey
   *   (optional) API key for authentication.
   * @param string $domain
   *   (optional) Domain for API key authentication, e.g., "example.com" or "any
   *   string, really".
   * @param int $startid
   *   (optional) Starting value for $id.
   */
  public function __construct($url, $version = '2.0', $apikey = NULL, $domain = NULL, $startid = 1) {
    $this->url = $url;
    $this->version = $version;
    $this->apikey = $apikey;
    $this->domain = $domain;
    $this->id = $startid;
  }

  /**
   * Calls a method on the declared JSON-RPC service.
   *
   * Throws an exception on HTTP errors. JSON-RPC errors are returned from
   * function without throwing an exception.
   *
   * @param string $method
   *   Service method to call, e.g., "system.connect".
   * @param array $params
   *   An associative array of parameters, e.g.,
   *   array("name" => "James T. Kirk").
   * @param int $sessid
   *   (optional) Session ID.
   * @param string $cookies
   *   (optional) A string of cookies to send in the request data.
   *
   * @return array
   *   The result of the JSON call as an associative array.
   *
   * @throws JsonRpcClientException
   */
  public function call($method, $params = array(), $sessid = NULL, $cookies = NULL) {
    $json_message = $this->jsonString($method, $params, $sessid);
    $this->debug($json_message);
    $headers = array('Content-Type' => 'application/json');
    if ($cookies) {
      $headers['Cookie'] = $cookies;
    }
    $http_result = drupal_http_request(
        $this->url,
        array(
          'headers' => $headers,
          'method' => 'POST',
          'data' => $json_message,
        )
      );
    $this->debug($http_result);
    // Check for HTTP error.
    if ($http_result->code != '200') {
      throw new JsonRpcClientHttpException(
        t("HTTP error: @code - @message",
          array('@code' => $http_result->code, '@message' => $http_result->error))
      );
    }

    $result = drupal_json_decode($http_result->data, TRUE);
    if (isset($http_result->headers['set-cookie'])) {
      $result['cookies'] = $http_result->headers['set-cookie'];
    }
    // Check for mismatched JSON ID.
    if ($this->checkId && !isset($result['error']) && $result['id'] != $this->id) {
      throw new JsonRpcClientHttpException(
        t("Mismatched JSON ID: @id",
          array('@id' => $result['id']))
      );
    }
    $this->debug($result);
    $this->id++;
    return ($result);
  }

  /**
   * Sets ID-checking to be enabled/disabled in the JSON response.
   *
   * Not advised to disable for anything other than testing.
   *
   * @param bool $check_id
   *   TRUE to enable ID checking on the JSON response; FALSE to disable it.
   */
  public function setIdCheck($check_id) {
    $this->checkId = $check_id;
  }

  /**
   * Sets the debug message type.
   *
   * Defaults to no debug messages (NULL).
   *
   * @param string $debug
   *   The type of debug message ('dsm', 'watchdog', NULL).
   */
  public function setDebug($debug = NULL) {
    $this->debug = $debug;
  }

  /**
   * Prints a debug message depending on the debug status value.
   *
   * @param string $message
   *   The message to be sent to output.
   */
  protected function debug($message) {
    switch ($this->debug) {
      case "dsm":
        if (module_exists('devel')) {
          dsm($message);
        }
        break;

      case "watchdog":
        ob_start();
        var_dump($message);
        $result = ob_get_clean();
        watchdog('jsonrpc_client', $result);
    }
  }

  /**
   * Builds a JSON-RPC call and returns it as a string.
   *
   * @param string $method
   *   Service method to call, e.g., "system.connect".
   * @param array $params
   *   An associative array of parameters, e.g.,
   *   array("name" => "James T. Kirk").
   * @param int $sessid
   *   (optional) The session ID.
   *
   * @return string
   *   A string containing the JSON call.
   */
  protected function jsonString($method, $params, $sessid = NULL) {
    // Build the message as an array.
    $message = array(
      'jsonrpc' => $this->version,
      'method' => $method,
    );
    if ($sessid) {
      $params['sessid'] = $sessid;
    }
    // TimeTrade server requires always including param array.
    $message['params'] = array_merge($this->getHash($method), $params);
    $message['id'] = $this->id;
    // Convert to a JSON string.
    $json_message = drupal_json_encode($message);
    return $json_message;
  }

  /**
   * Builds an associative array containing hash, domain, timestamp, nonce for
   * an API key authenticated service call.
   *
   * @param string $method
   *   Service method to call, e.g., "system.connect".
   *
   * @return array
   *   An array containing the calculated API key values.
   */
  protected function getHash($method) {
    $retval = array();
    if ($this->apikey) {
      $timestamp = (string) time();
      $nonce = user_password();
      $hash = hash_hmac(
        'sha256',
        $timestamp . ';' . $this->domain . ';' . $nonce . ';' . $method,
        $this->apikey
      );
      $retval = array(
        'hash' => $hash,
        'domain_name' => $this->domain,
        'domain_time_stamp' => $timestamp,
        'nonce' => $nonce,
      );
    }
    return $retval;
  }

}

/**
 * Custom exception thrown by JsonRpcClient on HTTP errors.
 */
class JsonRpcClientHttpException extends Exception {
}
