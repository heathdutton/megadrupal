<?php

/**
 * @file
 * The PoolParty PPX API-class.
 */

class SemanticConnectorCurlConnection {

  protected $endpoint;
  protected $credentials;
  protected $error;

  /**
   * The constructor of the PoolParty cURL connection class.
   *
   * @param string $endpoint
   *   URL of the endpoint of the PoolParty-server.
   * @param string $credentials
   *   Username and password if required (format: "username:password").
   */
  public function __construct($endpoint, $credentials = '') {
    $this->endpoint = $endpoint;
    $this->credentials = $credentials;
    $this->error = '';
  }

  /**
   * Get the endpoint of the cURL connection.
   *
   * @return string
   *   The URL of the endpoint.
   */
  public function getEndpoint() {
    return $this->endpoint;
  }

  /**
   * Get the last request error.
   *
   * @return string
   *   Error (code - message).
   */
  public function error() {
    return $this->error;
  }

  /**********************************************************
   * protected request functions
   **********************************************************/

  /**
   * Make a GET request.
   *
   * @param string $resource_path
   *   The path to the REST method. You can include wildcards in the string
   *   which will be filled in by the $parameters array. Ex: /Courses/%session.
   * @param array $variables
   *   An array of variables with the following keys:
   *   - parameters [optional] : Key/value pairs of parameters to inject into
   *     the resource path to replace dynamic values.
   *     Ex: array('%session' => 20111).
   *   - query [optional] : Key/value pairs of query string parameters.
   *     Ex: array('personid' => 2896263).
   *   - headers [optional] : Key/value pairs of extra header data to include
   *     in the request.
   *   - timeout [optional] : Timeout in seconds. Defaults to 30.
   *
   * @return object
   *   Returns an object containing the response data, FALSE otherwise.
   */
  public function get($resource_path, array $variables = array()) {
    $variables['headers']['Content-Type'] = 'application/json;charset=UTF-8';

    return $this->call($resource_path, $variables, 'GET');
  }

  /**
   * Make a POST request.
   *
   * @param string $resource_path
   *   The path to the REST method.
   * @param array $variables
   *   An array of variables.
   *
   * @return object
   *   Returns an object containing the response data, FALSE otherwise.
   */
  public function post($resource_path, array $variables = array()) {
    if (isset($variables['data'])) {
      if (is_string($variables['data'])) {
        $variables['headers']['Content-Length'] = strlen($variables['data']);
      }
    }
    else {
      $variables['data'] = array();
      $variables['headers']['Content-Length'] = 0;
    }

    return $this->call($resource_path, $variables, 'POST');
  }

  /**
   * Make a PUT request.
   *
   * @param string $resource_path
   *   The path to the REST method.
   * @param array $variables
   *   An array of variables.
   *
   * @return object
   *   Returns an object containing the response data, FALSE otherwise.
   */
  public function put($resource_path, array $variables = array()) {
    return $this->call($resource_path, $variables, 'PUT');
  }

  /**
   * Make a DELETE request.
   *
   * @param string $resource_path
   *   The path to the REST method.
   * @param array $variables
   *   An array of variables.
   *
   * @return object
   *   Returns an object containing the response data, FALSE otherwise.
   */
  public function delete($resource_path, array $variables = array()) {
    return $this->call($resource_path, $variables, 'DELETE');
  }

  /**
   * Basic request (Compatible with GET and DELETE).
   *
   * @param string $resource_path
   *   The path to the REST method.
   * @param array $variables
   *   An array of variables.
   * @param string $method
   *   The request-method (GET, POST, PUT, DELETE).
   *
   * @return object
   *   The response object or FALSE on error
   */
  protected function call($resource_path, array $variables = array(), $method = 'GET') {
    $variables['method'] = $method;

    // Initialize the cURL request.
    $ch = curl_init();

    // Set the default parameters for the cURL request.
    if (!isset($variables['headers']['Accept'])) {
      $variables['headers']['Accept'] = 'application/json';
    }
    $this->setRequestDefaults($ch, $variables);

    // Prepare the URL parameters.
    if (isset($variables['parameters']) && !empty($variables['parameters'])) {
      $this->prepareUrlParameters($resource_path, $variables['parameters']);
    }

    // Build the URL.
    $url = $this->buildUrl($resource_path, $variables);
    curl_setopt($ch, CURLOPT_URL, $url);

    switch ($method) {
      case 'GET':
        curl_setopt($ch, CURLOPT_HTTPGET, TRUE);
        break;

      case 'POST':
        curl_setopt($ch, CURLOPT_POST, 1);
        curl_setopt($ch, CURLOPT_POSTFIELDS, $variables['data']);
        break;

      case 'PUT':
        curl_setopt($ch, CURLOPT_CUSTOMREQUEST, 'PUT');
        curl_setopt($ch, CURLOPT_HTTPHEADER, array('X-HTTP-Method-Override: PUT'));
        curl_setopt($ch, CURLOPT_POSTFIELDS, $variables['data']);
        break;

      case 'DELETE':
        curl_setopt($ch, CURLOPT_CUSTOMREQUEST, 'DELETE');
        break;
    }

    // Make the request.
    $response = curl_exec($ch);

    $errorcode = curl_errno($ch);
    // There has been an error.
    if ($errorcode != 0) {
      // Log the error.
      $this->watchdog($method, $errorcode, curl_error($ch), $url);

      // Close the cURL request.
      curl_close($ch);

      return FALSE;
    }

    // Close the cURL request.
    curl_close($ch);

    // No error occurred, return the response.
    return $response;
  }

  /**
   * Set the default parameters for the cURL request.
   *
   * @param resource $ch
   *   The cURL request object.
   * @param array $variables
   *   Array of variables.
   */
  protected function setRequestDefaults($ch, array &$variables) {
    // Set the credentials.
    if (!empty($this->credentials)) {
      curl_setopt($ch, CURLOPT_USERPWD, $this->credentials);
    }

    // Set timeout.
    if (!(isset($variables['timeout']) && is_numeric($variables['timeout']) && intval($variables['timeout']) >= 0)) {
      $variables['timeout'] = 30;
    }
    curl_setopt($ch, CURLOPT_TIMEOUT, $variables['timeout']);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

    // Headers.
    if (isset($variables['headers']) && is_array($variables['headers'])) {

      $headers = array();
      foreach ($variables['headers'] as $key => $value) {
        $headers[] = trim($key) . ": " . trim($value);
      }
      curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
    }
  }

  /**
   * Encodes and appends any URL parameters to the resource path.
   *
   * @param string $resource_path
   *   Resource path.
   * @param array $parameters
   *   Array of URL parameters.
   */
  protected function prepareUrlParameters(&$resource_path, array $parameters) {
    // URL Encode all parameters.
    foreach ($parameters as $key => $param) {
      $parameters[$key] = urlencode($param);
    }

    // Add the parameters to the resource path.
    $resource_path = strtr($resource_path, $parameters);
  }

  /**
   * Build the URL to connect to.
   *
   * @param string $resource_path
   *   Base path.
   * @param array $variables
   *   Configuration variables.
   *
   * @return string
   *   Returns a full URL.
   */
  protected function buildUrl($resource_path, array $variables) {
    if (!empty($resource_path)) {
      $url = $this->endpoint . $resource_path;
    }
    else {
      $url = $this->endpoint;
    }

    // Set the options to be used by url().
    $options = array(
      'query' => isset($variables['query']) ? $variables['query'] : '',
      'absolute' => TRUE,
      'alias' => TRUE,
      'external' => TRUE,
    );

    // TODO: find a way to skip hook_url_outbound or migrate url() function
    // to internal function.
    $url = url($url, $options);

    return $url;
  }

  /**
   * Log error messages into the watchdog.
   *
   * @param string $method
   *   Request method.
   * @param int $code
   *   Error code.
   * @param string $error
   *   Error string.
   * @param string $url
   *   Request URL.
   * @param array $extra
   *   Extra data to show in the log.
   */
  protected function watchdog($method, $code, $error, $url, array $extra = array()) {
    $debug = "";
    if (!empty($extra)) {
      $debug .= "\n" . '<pre>' . print_r($extra, TRUE) . '</pre>';
    }
    $this->error = $code . ' - ' . $error;

    watchdog(
      'PoolParty Semantic Connector - PPX Api', 'Error making @method request (@code @message): @url !debug',
      array(
        '@method' => $method,
        '@code' => $code,
        '@message' => $error,
        '@url' => $url,
        '!debug' => $debug,
      ),
      WATCHDOG_ERROR
    );
  }
}
