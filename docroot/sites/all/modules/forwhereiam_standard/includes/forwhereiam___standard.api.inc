<?php
/**
 * @file
 * The file through which all HTTP requests are made to forWhereiAm APIs.
 */

class Forwhereiam {
  /**
   * Default options for curl.
   */
  public static $curlOpts = array(
    CURLOPT_CONNECTTIMEOUT => 10,
    CURLOPT_RETURNTRANSFER => TRUE,
    CURLOPT_TIMEOUT => 60,
    CURLOPT_HEADER => TRUE,
    CURLOPT_POST => 1,
    CURLOPT_USERAGENT => 'forwhereiam-php-1.0',
    CURLOPT_HTTPHEADER => array("Accept: application/json", 'Accept-Charset:utf-8'),
    CURLOPT_SSL_VERIFYPEER => FALSE,
  );

  /**
   * The client ID.
   *
   * @var string
   */
  protected $clientId;

  /**
   * The client Secret.
   *
   * @var string
   */
  protected $clientSecret;

  /**
   * Initialize a forwhereiam Application.
   *
   * The configuration:
   * - clientId: 	the client ID
   * - clientSecret: 	the client Secret
   *
   * @param array $config
   *   The application configuration array.
   */
  public function __construct($config = array()) {
    $this->clientId = $config['clientId'];
    $this->clientSecret = $config['clientSecret'];
  }

  /**
   * Get the client ID.
   *
   * @return string
   *   The Client ID.
   */
  public function getClientId() {
    return $this->clientId;
  }

  /**
   * Get the client Secret.
   *
   * @return string
   *   The Client Secret.
   */
  public function getClientSecret() {
    return $this->clientSecret;
  }

  /**
   * Make an API call.
   *
   * @param string $path
   *   The path (required).
   * @param string $method
   *   The http method (default 'GET').
   * @param array $params
   *   The query/post data.
   *
   * @return mixed
   *   The decoded response object.
   */
  public function api($path, $method = 'GET', $params = array()) {

    $params['client_id'] = $this->getClientId();
    $params['client_secret'] = $this->getClientSecret();

    if ($method == 'GET') {
      $path = $path . '?' . http_build_query($params);
      $params = array();
    }

    // Must json_encode all params values that are not strings.
    foreach ($params as $key => $value) {
      if (!is_string($value)) {
        $params[$key] = json_encode($value);
      }
    }

    $result = $this->makeRequest($this->getUrl($path), $params);

    // Split the HTTP response into header and body.
    list($headers, $body) = explode("\r\n\r\n", $result);
    unset($headers);
    if (empty($body)) {
      return FALSE;
    }

    $response_params = array();
    $response_params = json_decode($body, TRUE);

    if (!empty($response_params['error'])) {
      if ($response_params['error'] == 429) {
        watchdog('forwhereiam', $response_params['error_description'], array(), WATCHDOG_NOTICE, NULL);
      }
      elseif ($response_params['error'] == 401 || $response_params['error'] == 403 || $response_params['error'] == 404) {
        watchdog('forwhereiam', $response_params['error_description'], array(), WATCHDOG_ERROR, NULL);
      }
    }

    return $response_params;
  }

  /**
   * Makes an HTTP request.
   *
   * @param string $url
   *   The URL to make the request to.
   * @param array $params
   *   The parameters to use for the POST body.
   *
   * @return string
   *   The response text.
   */
  protected function makeRequest($url, $params = array()) {
    $ch = curl_init();

    $opts = self::$curlOpts;
    if (isset($params['media'])) {
      $opts[CURLOPT_POSTFIELDS] = $params;
    }
    else {
      $opts[CURLOPT_POSTFIELDS] = http_build_query($params, NULL, '&');
    }
    $opts[CURLOPT_URL] = $url;

    curl_setopt_array($ch, $opts);
    $result = curl_exec($ch);

    if ($result === FALSE) {
      $result = array('error' => curl_errno($ch), 'error_description' => curl_error($ch));
    }

    curl_close($ch);

    return $result;
  }

  /**
   * Build the URL for given domain alias, path and parameters.
   *
   * @param string $path
   *   Optional path (without a leading slash).
   * @param array $params
   *   Optional query parameters.
   *
   * @return string
   *   The URL for the given parameters
   */
  protected function getUrl($path = '', $params = array()) {
    $url = "https://api.forwhereiam.com/";
    if ($path) {
      if ($path[0] === '/') {
        $path = substr($path, 1);
      }
      $url .= $path;
    }
    if ($params) {
      $url .= '?' . http_build_query($params, NULL, '&');
    }

    return $url;
  }

}
