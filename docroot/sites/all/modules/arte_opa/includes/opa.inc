<?php
/**
 * @file
 * Objects, properties, and methods to communicate with the ARTE Open API
 */

/**
 * Ability to authorize and communicate with the ARTE Open API.
 */
class OPA {

  public $response;

  /**
   * Whether or not the API connection is in test mode.
   * @var boolean
   */
  protected $isTestMode = FALSE;

  protected $oAuthVersion = 'v2';

  /**
   * Enable/disable debug logging.
   * @var boolean
   */
  protected $debug = FALSE;

  /**
   * The URL when contacting OPA
   * @var string
   */
  protected $url = '';

  /**
   * Constructor which initializes the consumer.
   *
   * @param string $client_id
   *   ARTE client ID
   * @param string $client_secret
   *   ARTE client secret
   * @param string $token_never_expire (optional)
   *   ARTE auth token that never expires. Can be used to skip the roundtrip to fetch
   *   a valid token.
   */
  public function __construct($client_id, $client_secret, $token_never_expire = FALSE) {
    $this->client_id = $client_id;
    $this->client_secret = $client_secret;
    $this->token_never_expire = $token_never_expire;

    // Set the token that should never expire.
    if ($this->token_never_expire) {
      $this->setAccessToken($token_never_expire);
    }

    // Make configurable?
    $this->endpoint = 'https://api.arte.tv';
  }

  /**
   * Use the API in test mode.
   * No OAuth is needed for communication.
   */
  public function setTestMode() {
    $this->isTestMode = TRUE;
  }

  /**
   * Set debug mode for API calls.
   *
   * @param boolean $debug enable/disable debugging.
   */
  public function setDebug($debug = TRUE) {
    $this->debug = $debug;
  }

  /**
   * Get current debug mode.
   *
   * @return boolean
   */
  public function getDebug() {
    return $this->debug;
  }

  /**
   * Determine if this instance is fully configured.
   *
   * This will request a valid access token as well.
   */
  public function isAuthorized() {
    return !empty($this->client_id) && !empty($this->client_secret) && $this->requestToken();
  }

  /**
   * Make a call to the Open API.
   *
   * @param string $path
   *   Path to resource.
   * @param array $params
   *   Parameters to provide.
   *
   * @return mixed
   *   The requested response.
   *
   * @throws OPAException
   */
  public function apiCall($path, $params = array()) {
    if (!$this->getAccessToken()) {
      $this->refreshToken();
    }

    // We only support GET in OPA at the moment
    $method = 'GET';

    $this->response = $this->apiHttpRequest($path, $params, $method);

    switch ($this->response->code) {
      // The session ID or OAuth token used has expired or is invalid.
      case 401:
        // Refresh token.
        $this->refreshToken();

        // Rebuild our request and repeat request.
        $this->response = $this->apiHttpRequest($path, $params, $method);
        // Throw an error if we still have bad response.
        if (!in_array($this->response->code, array(200, 201, 204))) {
          throw new OPAException($this->response->error, $this->response->code);
        }

        break;

      case 200:
      case 201:
      case 204:
        // All clear.
        break;

      default:
        // We have problem and no specific error provided.
        if (empty($this->response->data)) {
          throw new OPAException($this->response->error, $this->response->code);
        }
    }

    $data = drupal_json_decode($this->response->data);
    if (!empty($data[0]) && count($data) == 1) {
      $data = $data[0];
    }

    // Catch and rethrow any errors reported by the API
    if (isset($data['error']) && isset($data['error_description'])) {
      throw new OPAException($data['error_description'], 0, null, $this->url);
    }

    if (isset($data['errors']) && isset($data['errors'][0]['title'])) {
      $code = isset($data['errors'][0]['code']) ? $data['errors'][0]['code'] : 0;
      $detail = isset($data['errors'][0]['detail']) ? $data['errors'][0]['detail'] : '';
      $status = isset($data['errors'][0]['status']) ? $data['errors'][0]['status'] : 'N/A';

      $error_msg = t('Failed fetching video !programId for platform !platform. !title, !detail. Status: !status', array('!programId' => $params['programId'], '!title' => $data['errors'][0]['title'], '!platform' => $params['platform'], '!detail' => $detail, '!status' => $status));

      throw new OPAException($error_msg, $code, null, $this->url);
    }

    if (!empty($data['errorCode'])) {
      throw new OPAException($data['message'], 0, null, $this->url);
    }

    // Final check, make sure we have received videos
    if (!isset($data['videos'])) {
      throw new OPAException(t('Expected attribute "videos" not found in OPA response.', 0, null, $this->url));
    }

    return $data;
  }

  /**
   * Private helper to issue an Open API request.
   *
   * @param string $path
   *   Path to resource.
   * @param array $params
   *   Parameters to provide.
   * @param string $method
   *   Method to initiate the call, such as GET or POST.
   *
   * @return object
   *   The requested data.
   */
  protected function apiHttpRequest($path, $params, $method) {
    $url = $this->getApiEndPoint() . $path;

    // Add access token to the parameters
    $params['access_token'] = $this->getAccessToken();
    $data = drupal_http_build_query($params);

    // Build the request, including path and headers.
    $options = array(
      'method' => $method,
      'headers' => array(
        'User-Agent' => variable_get('arte_opa_user_agent', 'Drupal : ' . variable_get('site_name', 'unknown'))
      ),
    );

    // Allow other modules to alter query headers.
    drupal_alter('arte_opa_headers', $options['headers'], $params);

    return $this->httpRequest($url, $data, $options, $method);
  }

  /**
   * Make the HTTP request. Wrapper around drupal_http_request().
   *
   * @param string $url
   *   Path to make request from.
   * @param array $data
   *   The request body.
   * @param array $options
   *   The request options.
   *
   * @return object
   *   OPA response object.
   */
  protected function httpRequest($url, $data, $options) {
    // Append the parameters directly to the URL
    $url = $url . '?' . $data;
    if ($this->getDebug()) {
      $msg = 'OPA request to !url';
      if (drupal_is_cli()) {
        drush_log(dt($msg, array('!url' => $url)), 'status');
      }
      else {
        drupal_set_message(t($msg, array('!url' => l($url, $url))), 'info', FALSE);
      }
    }

    // Also add the latest api call to this object so it can be retrieved for logging in a later stage
    $this->url = $url;

    return drupal_http_request($url, $options);
  }

  /**
   * Get the API end point for a given type of the API.
   *
   * @return string
   *   Complete URL endpoint for API access.
   */
  public function getApiEndPoint() {
    return $this->endpoint . '/api/opa/v2/';
  }

  /**
   * Get the access token.
   */
  public function getAccessToken() {
    return variable_get('arte_opa_access_token', '');
  }

  /**
   * Set the access token.
   *
   * @todo  consider setting this in the SESSION instead of the database, since it'll expire soon (expires after an hour)
   *
   * @param string $token
   *   Access token from ARTE.
   */
  protected function setAccessToken($token) {
    variable_set('arte_opa_access_token', $token);
  }

  /**
   * Refresh access token based on the refresh token. Updates session variable.
   *
   * @throws OPAException
   */
  protected function refreshToken() {
    // For now this is equivalent to requesting a new token, i.e. the refresh token
    // logic is not yet needed.
    // As documented in: https://github.com/ArteGEIE/API-DOC/blob/master/OAuthAPI/README.md
    $this->requestToken();
  }

  /**
   * OAuth: request an access token.
   */
  public function requestToken() {
    // Check if we use a never expire token; if we do, we shouldn't request a new token.
    if ($this->token_never_expire) {
      return TRUE;
    }

    $data = drupal_http_build_query(array(
      // We are using the preferred client_credentials method. See https://github.com/ArteGEIE/API-DOC/tree/master/oauth-server#generating-an-access-token
      'grant_type' => 'client_credentials',
      'client_id' => $this->client_id,
      'client_secret' => $this->client_secret,
    ));

    $url = $this->endpoint . '/api/oauth/' . $this->oAuthVersion . '/token';
    $response = $this->httpRequest($url, $data);

    $data = drupal_json_decode($response->data);

    if ($response->code != 200) {
      $error = isset($data['error_description']) ? $data['error_description'] : $response->error;
      throw new OPAException($error, $response->code);
    }

    $this->setAccessToken($data['access_token']);

    return TRUE;
  }

  /**
   * Query for the video resources in OPA.
   * See https://github.com/ArteGEIE/API-DOC/blob/master/OpenAPI/Resources/videos.md
   *
   * @param  array  $params [description]
   * @return [type]         [description]
   */
  public function videos($params = array()) {
    return $this->apiCall('videos', $params);
  }

}

/**
 * Exception class
 */
class OPAException extends Exception {

  private $url;

  public function __construct($message = null, $code = 0, Exception $previous = null, $url = null) {
    parent::__construct($message, $code, $previous);
    $this->url = $url;
  }

  /**
   * Retrieve the OPA url that caused this exception. Useful for logging.
   *
   * @return [type] [description]
   */
  public function getUrl() {
    return $this->url;
  }

}
