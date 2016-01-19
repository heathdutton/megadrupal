<?php

/**
 * @file
 * Handle the base functionality for querying the Pin API.
 */


/**
 * A simple wrapper class for our endpoints.
 */
class CommercePinEndpoints {
  const ENDPOINT_LIVE = 'api.pin.net.au/1';
  const ENDPOINT_TEST = 'test-api.pin.net.au/1';
}


/**
 * The base class for a Pin transaction.
 */
abstract class CommercePinTransactionBase {

  protected $secretKey = FALSE;
  protected $testMode = TRUE;

  /**
   * Create a transaction with a key and a test mode.
   *
   * @param String $secret_key
   *   The secret key used to query the API.
   * @param Boolean $test_mode
   *   If we are going to query the API in test mode.
   */
  public function __construct($secret_key, $test_mode = FALSE) {
    $this->secretKey = $secret_key;
    $this->testMode = $test_mode;
  }

  /**
   * Check if the request to the gateway will be in test mode.
   *
   * @return Boolean
   *   A test mode flag.
   */
  public function isTestMode() {
    return $this->testMode;
  }

  /**
   * Get the URL we will be querying.
   *
   * @return String
   *   The endpoint for the transaction including the authentication details.
   */
  public function getApiBase() {

    $url = $this->testMode
      ? CommercePinEndpoints::ENDPOINT_TEST : CommercePinEndpoints::ENDPOINT_LIVE;

    return 'https://' . $this->secretKey . ':@' . $url;
  }

  /**
   * Query the Pin API.
   *
   * @param String $endpoint
   *   The endpoint we are querying.
   * @param array $variables
   *   A list of variables to send to the gateway.
   *
   * @return CommercePinResponse
   *   A response object containing information about the request.
   */
  public function queryPinApi($endpoint, $variables) {

    // The endpoint URL we will query.
    $endpoint_url = $this->getApiBase() . '/' . $endpoint . '/';

    // Add some standard variables to our request variables.
    $request_variables = array(
      'ip_address' => ip_address(),
    ) + $variables;

    // Post to our endpoint with all our variables.
    $response = $this->jsonPostRequest($endpoint_url, $request_variables);

    // Return a newly constructed response object.
    return new CommercePinResponse($response);

  }


  /**
   * Make a JSON post request.
   *
   * @param String $url
   *   The URL to POST to.
   * @param array $data
   *   An array of data to be converted and posted as JSON.
   *
   * @return array|Object
   *   An array representing the response from the server.
   */
  protected function jsonPostRequest($url, $data) {
    $encoded_request_variables = json_encode($data);
    $http_response = drupal_http_request(
      $url,
      array(
        'method' => 'POST',
        'headers' => array(
          'Content-Length' => strlen($encoded_request_variables),
          'Content-Type' => 'application/json',
        ),
        'data' => $encoded_request_variables,
      )
    );

    return json_decode($http_response->data);
  }

  /**
   * Make a JSON delete request.
   *
   * @param String $url
   *   The URL to DELETE to.
   *
   * @return bool
   *   TRUE if successful.
   */
  protected function jsonDeleteRequest($url) {
    $http_response = drupal_http_request(
      $url,
      array(
        'method' => 'DELETE',
        'headers' => array(
          'Content-Type' => 'application/json',
        ),
      )
    );

    return $http_response->code == 204;
  }

}
