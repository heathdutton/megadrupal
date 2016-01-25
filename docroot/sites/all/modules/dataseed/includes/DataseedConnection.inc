<?php
/**
 * @file
 * Defines DataseedConnection class which handles the server-to-server
 * communication to the Dataseed Server.
 */

class DataseedConnection {

  /**
   * The Dataseed server's URL
   *
   * @var string
   */
  protected $baseUrl;

  /**
   * The options passed when creating this connection.
   *
   * @var array
   */
  protected $options;

  /**
   * HTTP Basic Authentication header to set for requests to the Dataseed
   * server.
   *
   * @var string
   */
  protected $httpAuth;

  /**
   * Set the required options for a new DataseedConnection.
   *
   * Valid options include:
   *   - scheme: Scheme of the base URL of the Dataseed server.
   * Most probably "http" or "https". Defaults to "http".
   *   - host: The host name (or IP) of the Dataseed server. Defaults to
   *     "dataseedapp.com".
   *   - port: The port of the Dataseed server. Defaults to 80.
   *   - path: The base path to the Dataseed server. Defaults to "/".
   *   - http_user: If both this and "http_pass" are set, will use this
   *     information to add basic HTTP authentication to all requests to the
   *     Dataseed server. Not set by default.
   *   - http_pass: See "http_user".
   *   - http_method: The HTTP method to use for communicating to Dataseed. Can
   *     be either "GET", "POST", "DELETE", "HEAD", "PUT" (check the Dataseed
   *     API for further information https://dataseedapp.com/documentation)
   *     Defaults to "POST".
   */
  public function __construct(array $options) {
    $options += array(
      'scheme'      => 'http',
      'host'        => 'dataseedapp.com',
      'port'        => 80,
      'path'        => '/',
      'http_user'   => NULL,
      'http_pass'   => NULL,
      'http_method' => 'POST',
    );
    $this->options = $options;

    $path = (trim($options['path']) == '/') ?
      $options['path'] :
      '/' . trim($options['path'], '/') . '/';

    $this->baseUrl = $options['scheme'] . '://' . $options['host'] . ':' . $options['port'] . $path;

    // Set HTTP Basic Authentication parameter, if login data was set.
    if (strlen($options['http_user']) && strlen($options['http_pass'])) {
      $this->httpAuth = 'Basic ' . base64_encode($options['http_user'] . ':' . $options['http_pass']);
    }

  }

  /**
   * Checks whether or not a dataset exists on Dataseed.
   *
   * @param string $dataset_id
   *   the dataset id to be checked
   *
   * @return bool
   *   TRUE if the dataset exists, FALSE otherwise
   */
  public function datasetExists($dataset_id) {
    $options = array(
      'method' => 'HEAD',
    );

    $url      = $this->baseUrl . "api/datasets/$dataset_id";
    $response = $this->makeHttpRequest($url, $options);

    // The HEAD API will return a 200 response code if the dataset exists on
    // Dataseed, 404 otherwise.
    try {
      return $this->checkResponse($response);
    }
    catch (SearchApiException $e) {
      return !$this->checkResponse($response, 404);
    }
  }

  /**
   * A wrapper around drupal_http_request() for sending requests to Dataseed.
   */
  protected function makeHttpRequest($url, array $options = array()) {
    if ($this->httpAuth) {
      $options['headers']['Authorization'] = $this->httpAuth;
    }

    $options['headers']['Content-Type'] = $options['headers']['Accept'] = 'application/json';

    $result = drupal_http_request($url, $options);

    if (!isset($result->code) || $result->code < 0) {
      $result->code           = 0;
      $result->status_message = 'Request failed';
      $result->protocol       = 'HTTP/1.0';
    }
    // Additional information may be in the error property.
    if (isset($result->error)) {
      $result->status_message .= ': ' . check_plain($result->error);
    }

    if (!isset($result->data)) {
      $result->data     = '';
      $result->response = NULL;
    }
    else {
      $response = json_decode($result->data);
      if (is_object($response)) {
        foreach ($response as $key => $value) {
          $result->$key = $value;
        }
      }
    }

    return $result;
  }

  /**
   * Checks the response code.
   *
   * @param object $response
   *   A response object.
   * @param int $success_response_code
   *   The response code indicating that the request has been successfully
   *   processed by dataseed
   *
   * @throws SearchApiException
   *   if the response code is not equal to $successResponseCode
   * @return TRUE
   *   if the request returns a $successResponseCode
   */
  protected function checkResponse($response, $success_response_code = 200) {
    $code = (int) $response->code;

    if ($code != $success_response_code) {
      // Throw an Exception with detailed info.
      $response->status_message .= $response->data;
      throw new SearchApiException('"' . $code . '" Status: ' . $response->status_message);
    }

    return TRUE;
  }

}
