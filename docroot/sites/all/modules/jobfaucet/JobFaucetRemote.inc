<?php

/**
 * @file
 * Drupal-specific object definition for working with the JobFaucet API.
 */

class JobFaucetRemote {
  public $gateway = 'http://jobfaucet.com/';
  public $apiVersion = '0.1';
  public $publicKey;
  public $privateKey;

  /**
   * Number of seconds before a request to the JobFaucet server times out.
   *
   * By default, this would be set to 30, which is way too long to wait for a
   * node to save or update. Ultimately, this may need to be tweaked based on
   * how responsive the JobFaucet server is.
   *
   * @var float
   */
  public $requestTimeout = 10.0;

  /**
   * Constructor.
   */
  public function __construct() {
    $this->publicKey = variable_get('jobfaucet_public_key');
    $this->privateKey  = variable_get('jobfaucet_private_key');
  }

  /**
   * Send an outgoing request to JobFaucet.com.
   *
   * @param string $url
   *   The full URL to send the request to (including both the domain and
   *   REST resource path).
   * @param string $method
   *   The HTTP method to use (e.g. 'GET', 'POST', 'PUT', 'DELETE').
   * @param string $op
   *   The operation being performed (one of 'create', 'update', 'delete', or
   *   'verify').
   * @param array $data
   *   A structured array of data to send in the request.
   * @param bool $add_auth
   *   TRUE if authentication headers should be added.
   *
   * @return obj
   *   As returned by drupal_http_request, with the following additional
   *   properties:
   *     - succeeded: TRUE if the requested succeeded, otherwise FALSE.
   *     - jf_message: A success or failure message that can optionally be
   *       shown to the user.
   */
  public function request($url, $method, $op, $data = NULL, $add_auth = FALSE) {

    $headers['Content-Type'] = 'application/json';
    $query = json_encode($data);

    if ($add_auth) {
      if (!empty($this->privateKey) && !empty($this->publicKey)) {

        // Add Oauth headers to the request.
        $this->authenticateRequest($url, $method, $headers, $query);

      }
      else {
        // Keys are empty, so no need to contact server to know that
        // authentication won't succeed.
        $response = new stdClass();
        $response->code = 401;
        $response->succeeded = FALSE;
        $response->error = $response->jf_message = t('Your JobFaucet API keys are empty.  Please add keys to use the JobFaucet service.');
        return $response;
      }
    }

    $options = array(
      'headers' => $headers,
      'method' => $method,
      'data' => $query,
      'timeout' => $this->requestTimeout,
    );

    $response = drupal_http_request($url, $options);

    if (isset($response->data)) {
      $response->data = json_decode($response->data, TRUE);
    }

    // We first check to see if $response->error has been set by
    // drupal_http_request(), which will be the case if the response code
    // is anything other than 200 or 304.
    if (isset($response->error)) {
      // A message in the data array takes precedence over the HTTP
      // error message.
      $response->jf_message = isset($response->data['message']) ?
        $response->data['message'] : $response->error;

      $response->succeeded = FALSE;
    }
    // By design, a successful response should always return a message in the
    // data array.  If it didn't, but we received a success status code,
    // something's not quite right on the server end.
    elseif (!isset($response->data['message'])) {
      $response->jf_message = t('Unknown Jobfaucet error.  Please try again later or contact JobFaucet.');
      $response->succeeded = FALSE;
    }
    else {
      $response->jf_message = $response->data['message'];
      $response->succeeded = TRUE;
    }

    if (!$response->succeeded) {

      // Log any error messages.
      $vars = array(
        '%code' => $response->code,
        '%message' => $response->jf_message,
      );
      watchdog('jobfaucet', 'JobFaucet request failed with status code %code: %message', $vars, WATCHDOG_WARNING);

      // Tweak the displayable error messages based on the context
      // of the request.
      switch ($op) {
        case 'update':
        case 'delete':
        case 'create':
          $response->jf_message = t('Attempt to @op job on JobFaucet.com failed for the following reason: @reason',
            array('@op' => $op, '@reason' => $response->jf_message));
          break;

        case 'verify':
          if (!isset($response->data['message'])) {
            $response->jf_message = t('Unable to contact JobFaucet server to verify API keys.');
          }
          break;
      }
    }

    return $response;
  }


  /**
   * Add an Oauth authorization header to a request.
   *
   * @param array $headers
   *   An associative array of headers passed by reference.
   * @param string $query
   *   A string of query parameters as formatted by drupal_http_build_query.
   *
   * @see JobFaucetRemote::request()
   */
  public function authenticateRequest($url, $method, &$headers, $query) {
    $consumer = new DrupalOAuthConsumer($this->publicKey, $this->privateKey);

    // Get the oauth parameters and sign the request.
    $request = DrupalOAuthRequest::from_consumer_and_token($consumer, NULL, $method, $url, NULL);
    $signature_method = new OAuthSignatureMethod_HMAC('sha1');
    $request->sign_request($signature_method, $consumer, NULL);

    // Build the oauth headers.
    $oauth_header = $request->to_header();
    $oauth_header = preg_replace('/^Authorization: /', '', $oauth_header);

    $headers['Authorization'] = $oauth_header;
  }

  /**
   * Send a new job to JobFaucet.com.
   *
   * @param array $data
   *   An associative array of fields describing this job.
   *
   * @return object
   *   The server response.
   */
  public function createJob($data) {
    $url = $this->gateway . $this->apiVersion . '/job';
    $response = $this->request($url, 'POST', 'create', $data, TRUE);

    return $response;
  }

  /**
   * Update a job that has already been sent to JobFaucet.com.
   *
   * @param array $data
   *   An associative array of fields describing this job.
   * @param int $job_id
   *   The Jobfaucet.com ID of this job (sent in the server response when a job
   *   is first created).
   *
   * @return object
   *   The server response.
   */
  public function updateJob($data, $job_id) {
    $url = $this->gateway . $this->apiVersion . '/job/' . $job_id;
    $response = $this->request($url, 'PUT', 'update', $data, TRUE);

    return $response;
  }

  /**
   * Delete a job on JobFaucet.com.
   *
   * @param int $job_id
   *   The Jobfaucet.com ID of this job (sent in the server response when a job
   *   is first created).
   *
   * @return object
   *   The server response.
   */
  public function deleteJob($job_id) {
    $url = $this->gateway . $this->apiVersion . '/job/' . $job_id;
    $response = $this->request($url, 'DELETE', 'delete', NULL, TRUE);

    return $response;
  }

  /**
   * Verify Oauth keys.
   *
   * This is only necessary during configuration since all requests are
   * authenticated when they are made.
   */
  public function verifyAccount() {
    $url = $this->gateway . $this->apiVersion . '/account/verify';
    $response = $this->request($url, 'POST', 'verify', NULL, TRUE);

    return $response;
  }
}
