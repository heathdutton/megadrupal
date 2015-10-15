<?php

/**
 * @file
 * A mock OAuth 2.0 client for use with tests.
 */

/**
 * Implements a test OAuth 2.0 client.
 *
 * Used for testing page callbacks.
 */
class BlizzardApiLoginTestClient extends BlizzardApiLoginClient {
  const AUTHORIZE_PATH = 'oauth/authorize';
  const TOKEN_PATH = 'oauth/token';
  
  protected $token;
  
  /**
   * Constructs a BlizzardApiLoginTestClient object.
   *
   * @param string $region
   *   A supported Battle.net login region.
   * @param array $scopes
   *   (optional) Not used.
   */
  public function __construct($region, $scopes = array()) {
    parent::__construct($region, $scopes);
    
    $this->client = new stdClass();
    $this->client->client_id = variable_get('blizzardapi_public_key', '');
    $this->client->client_secret = variable_get('blizzardapi_private_key', '');
  }
  
  /**
   * Returns a sample token. Does not authenticate anything.
   */
  public function authenticate($code) {
    // The test module just returns a sample token, no POST data is required.
    $url = url(self::TOKEN_PATH, array('absolute' => TRUE));
    $context = stream_context_create(array('ssl' => array('allow_self_signed' => TRUE)));
    $response = drupal_http_request($url, array('context' => $context));
    $this->token = json_decode($response->data, TRUE);
  }
  
  /**
   * {@inheritdoc}
   */
  public function getAccessToken() {
    return $this->token;
  }
  
  /**
   * {@inheritdoc}
   */
  public function getAccessTokenScopes() {
    return $this->token['scope'];
  }
  
  /**
   * {@inheritdoc}
   */
  public function getAccountId() {
    // Simulate multiple accounts.
    switch($this->region) {
      case BlizzardApiLoginClient::REGION_EU:
        return 1001;
      case BlizzardApiLoginClient::REGION_KR:
        return 1002;
      default:
        return 1000;
    }
  }

  /**
   * Returns a link to the test login page.
   */
  public function getAuthorizeUrl($state = '') {
    $query = array(
      'client_id' => $this->client->client_id,
      'redirect_uri' => $this->redirectUrl,
      'response_type' => 'code'
    );
    
    if (!empty($this->scopes)) {
      $query['scope'] = implode(' ', $this->scopes);
    }
    if (!empty($state)) {
      $query['state'] = $state;
    }
    
    return url(self::AUTHORIZE_PATH, array('query' => $query, 'absolute' => TRUE));
  }
  
  /**
   * {@inheritdoc}
   */
  public function getClient() {
    return $this->client;
  }

  /**
   * {@inheritdoc}
   */
  public function isAccessTokenExpired() {
    return FALSE;
  }
}

/**
 * Is not an implementation of BlizzardApiLoginClient.
 *
 * Used to test that blizzardapi_login_client() does not return random objects.
 */
class BlizzardApiLoginInvalidClient {
  public function __construct($region, $scopes = array()) {}
}

/**
 * Enables testing of the default client's implementation.
 */
class BlizzardApiLoginDefaultClientTest extends BlizzardApiLoginDefaultClient {
  /**
   * Removes the requirement for a CA bundle to be set on testing environments.
   */
  protected function createContext($name = '') {
    return NULL;
  }

  /**
   * Redirect requests back to the test module.
   */
  public function getBaseUrl() {
    return rtrim(url('', array('absolute' => TRUE)), '/');
  }

  /**
   * Allows changing the access token to test validation.
   */
  public function setAccessToken($token) {
    $this->token = $token;
  }
}

/**
 * Redirects requests back to the test module.
 *
 * Used for testing the default client's sendRequest() method.
 */
class BlizzardApiTestRequest extends BlizzardApiRequest {
  /**
   * Overrides BlizzardApiRequest::getBaseUrl().
   */
  protected function getBaseUrl() {
    return rtrim(url('', array('absolute' => TRUE)), '/');
  }
}
