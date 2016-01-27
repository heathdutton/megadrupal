<?php

/**
 * @file
 * Implements classes used to login using the Battle.net service.
 */

/**
 * Represents an OAuth 2.0 client that can be used to authenticate a user.
 */
interface BlizzardApiLoginClientInterface {
  /**
   * Authenticate a user and retrieve an access token.
   *
   * @param string $code
   *   An authorization code that was returned to the redirect URL.
   */
  public function authenticate($code);
  
  /**
   * Get the underlying OAuth 2.0 client object.
   */
  public function getClient();
  
  /**
   * Get an array containing the access token or NULL if no token is available.
   */
  public function getAccessToken();
  
  /**
   * Get an array containing each scope that the user has granted permission for.
   */
  public function getAccessTokenScopes();
  
  /**
   * Get the URL to the authorization endpoint.
   *
   * @param string $state
   *   (optional) The state parameter passed to the client. This should be used
   *   for CSRF protection.
   */
  public function getAuthorizeUrl($state = '');

  /**
   * Returns TRUE if an access token is expired or not set.
   */
  public function isAccessTokenExpired();
}

/**
 * Performs basic tasks required by an OAuth 2.0 client including checking for
 * authentication keys, and obtaining the redirect URL used during the
 * authorization flow.
 */
abstract class BlizzardApiLoginClient implements BlizzardApiLoginClientInterface {
  const REGION_US = 'us';
  const REGION_EU = 'eu';
  const REGION_KR = 'kr';
  const REGION_TW = 'tw';
  const REGION_CN = 'cn';
  
  protected $client;
  
  protected $redirectUrl;
  
  protected $region;
  
  protected $scopes;
  
  /**
   * Constructs a BlizzardApiLoginClient object.
   *
   * @param string $region
   *   A supported Battle.net login region.
   * @param array $scopes
   *   (optional) A list of scopes to request during the login attempt.
   */
  public function __construct($region, $scopes = array()) {
    if (empty($region)) {
      throw new InvalidArgumentException('Region cannot be empty');
    }
    if (!blizzardapi_check_authentication()) {
      throw new BlizzardApiLoginException('Authentication keys are required');
    }
    
    $this->region = $region;
    $this->scopes = $scopes;
    $this->redirectUrl = url(BLIZZARDAPI_LOGIN_PATH, array('absolute' => TRUE));
    $this->redirectUrl = str_replace('http://', 'https://', $this->redirectUrl);
  }

  /**
   * Gets the Battle.net account ID associated with the current access token.
   *
   * @return mixed
   *   An account ID or FALSE if the client encountered a problem while making
   *   the request.
   */
  abstract function getAccountId();
}

/**
 * Implements a basic OAuth 2.0 client.
 *
 * This class can only perform the authorization code flow.
 */
class BlizzardApiLoginDefaultClient extends BlizzardApiLoginClient {
  const BATTLENET_URL_BASE = 'https://{region}.battle.net';
  const BATTLENET_URL_BASE_CHINA = 'https://www.battlenet.com.cn';
  
  const BATTLENET_AUTHORIZE_PATH = '/oauth/authorize';
  const BATTLENET_TOKEN_PATH = '/oauth/token';
  
  const BATTLENET_NAME = 'www.battle.net';
  const BATTLENET_NAME_CHINA = '*.battlenet.com.cn';
  
  protected $client_id;
  
  protected $client_secret;
  
  protected $token;
  
  /**
   * Constructs a BlizzardApiLoginDefaultClient object.
   *
   * @param string $region
   *   A supported Battle.net login region.
   * @param array $scopes
   *   (optional) A list of scopes to request during the login attempt.
   */
  public function __construct($region, $scopes = array()) {
    parent::__construct($region, $scopes);
    
    $this->client_id = variable_get('blizzardapi_public_key', '');
    $this->client_secret = variable_get('blizzardapi_private_key', '');
  }
  
  /**
   * Authenticate a user and retrieve an access token.
   * 
   * @param string $code
   *   An authorization code that was returned to the redirect URL.
   * @param array $options
   *   (optional) An array containing additional options that are passed to
   *   drupal_http_request().
   *
   * @throws BlizzardApiLoginAuthException
   */
  public function authenticate($code, $options = array()) {
    if (empty($code)) {
      throw new BlizzardApiLoginAuthException('Invalid code');
    }
    
    $data = array(
      'code' => $code,
      'grant_type' => 'authorization_code',
      'redirect_uri' => $this->redirectUrl,
      'client_id' => $this->client_id,
      'client_secret' => $this->client_secret
    );
    
    // Set default options.
    $options += array(
      'headers' => array(),
      'context' => $this->createContext()
    );
    
    $options['headers']['Content-Type'] = 'application/x-www-form-urlencoded';
    $options['headers']['User-Agent'] = BLIZZARDAPI_LOGIN_UA;
    unset($options['headers']['Accept-Encoding']);
    
    $request = array(
      'headers' => $options['headers'],
      'method' => 'POST',
      'data' => http_build_query($data, '', '&')
    );
    
    $response = drupal_http_request($this->getBaseUrl() . self::BATTLENET_TOKEN_PATH, $request + $options);
    if ($response->code == 200) {
      $this->token = json_decode($response->data, TRUE);
      if (!isset($this->token['created'])) {
        $this->token['created'] = time();
      }
    }
    else {
      $message = 'Unable to retrieve access token';
      if (isset($response->data)) {
        $data = json_decode($response->data, TRUE);
        if (isset($data) && !empty($data['error'])) {
          $message = $data['error'] . (!empty($data['error_description']) ? ': ' . $data['error_description'] : '');
        }
      }
      else {
        $message = $response->error;
      }
      throw new BlizzardApiLoginAuthException($message, $response->code);
    }
  }
  
  /**
   * Creates a stream context resource for secure connections.
   *
   * @param string $name
   *   (optional) The server name to match against the certificate.
   *
   * @throws BlizzardApiLoginAuthException
   */
  protected function createContext($name = '') {
    $cafile = variable_get('blizzardapi_login_cafile', '');
    $ciphers = variable_get('blizzardapi_login_ciphers', BLIZZARDAPI_LOGIN_CIPHERS_MOZ_INTERMEDIATE);
    
    if (version_compare(phpversion(), '5.6.0', '<')) {
      if (empty($cafile)) {
        throw new BlizzardApiLoginAuthException('CA bundle is required');
      }
      if (empty($name)) {
        // No support for the perfectly valid SAN attribute...
        $name = $this->region == self::REGION_CN ? self::BATTLENET_NAME_CHINA : self::BATTLENET_NAME;
      }
      $ssl_context = array(
        'verify_peer' => TRUE,
        'verify_depth' => 5,
        'cafile' => $cafile,
        'CN_match' => $name,
        'SNI_enabled' => TRUE,
        'ciphers' => $ciphers,
        'disable_compression' => TRUE  // Requires PHP 5.4.13
      );
    }
    else {
      if (empty($name)) {
        $name = parse_url($this->getBaseUrl(), PHP_URL_HOST);
      }
      $ssl_context = array(
        'verify_peer' => TRUE,
        'verify_depth' => 5,
        'peer_name' => $name,
        'SNI_enabled' => TRUE,
        'ciphers' => $ciphers,
        'disable_compression' => TRUE
      );
      // If not provided, OS-managed certificates will be used.
      if (!empty($cafile)) {
        $ssl_context['cafile'] = $cafile;
      }
    }
    
    return stream_context_create(array('ssl' => $ssl_context));
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
    $data = array();
    $request = BlizzardApiRequestFactory::getAccount($this->region);
    try {
      $data = $this->sendRequest($request);
    }
    catch (Exception $e) {
      watchdog_exception('blizzardapi_login', $e);
    }
    if (isset($data['id'])) {
      return $data['id'];
    }
    return FALSE;
  }

  /**
   * {@inheritdoc}
   */
  public function getAuthorizeUrl($state = '') {
    $query = array(
      'client_id' => $this->client_id,
      'redirect_uri' => $this->redirectUrl,
      'response_type' => 'code'
    );
    
    if (!empty($this->scopes)) {
      $query['scope'] = implode(' ', $this->scopes);
    }
    if (!empty($state)) {
      $query['state'] = $state;
    }
    
    return url($this->getBaseUrl() . self::BATTLENET_AUTHORIZE_PATH, array('query' => $query));
  }
  
  /**
   * Get the base URL to used during authentication.
   */
  protected function getBaseUrl() {
    if ($this->region == BlizzardApiLoginClient::REGION_CN) {
      return self::BATTLENET_URL_BASE_CHINA;
    }
    return str_replace('{region}', $this->region, self::BATTLENET_URL_BASE);
  }
  
  /**
   * Returns NULL. This client does not use an underlying OAuth 2.0 implementation.
   */
  public function getClient() {
    return NULL;
  }

  /**
   * {@inheritdoc}
   */
  public function isAccessTokenExpired() {
    if (empty($this->token) || !isset($this->token['created'])) {
      return TRUE;
    }
    $expires_in = isset($this->token['expires_in']) ? (int) $this->token['expires_in'] : 0;
    // If the access token is a few seconds from expiring, return TRUE so that
    // a refresh token can be used to update it before any requests are made.
    $timestamp = $this->token['created'] + $expires_in - 30;
    if ($timestamp < time()) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Performs an authenticated GET request.
   *
   * @param BlizzardApiRequest $request
   *   The API request to perform.
   *
   * @return array
   *   An array containing the requested data.
   *
   * @throws BlizzardApiException, BlizzardApiAuthException
   */
  public function sendRequest($request) {
    if (!($request instanceof BlizzardApiRequest)) {
      throw new InvalidArgumentException('Invalid request object');
    }

    // For the time being, this method is intentionally hard-coded to only
    // support GET requests instead of using $request->getMethod().
    $options = array(
      'context' => $this->createContext($request->getPeerName()),
      'headers' => array(),
      'method' => 'GET',
      'max_redirects' => 0
    );

    if ($this->isAccessTokenExpired()) {
      // Normally, this is where we use a refresh token to update the access
      // token, but Blizzard does not support refresh tokens.
      throw new BlizzardApiLoginAuthException('Access token has expired');
    }

    $options['headers']['Authorization'] = 'Bearer ' . $this->token['access_token'];
    $options['headers']['User-Agent'] = BLIZZARDAPI_LOGIN_UA;

    $response = drupal_http_request($request->getUrl(), $options);
    if ($response->code == 200) {
      $data = json_decode($response->data, TRUE);
      if ($data === NULL) {
        throw new BlizzardApiLoginException('Unable to decode JSON response');
      }
      return $data;
    }
    if ($response->code >= 300) {
      $code = $response->code;
      $body = $response->data;

      $data = json_decode($response->data, TRUE);
      // Got error data from the Mashery server.
      if ($data !== NULL && isset($data['error'])) {
        if (isset($data['error_description'])) {
          $body = $data['error_description'];
        }
      }
      // Got error data from the Blizzard server.
      if ($data !== NULL && isset($data['detail'])) {
        $body = $data['detail'];
      }

      $message = sprintf('Invalid response from %s: (%d) %s', $request->getUrl(), $code, $body);
      throw new BlizzardApiLoginException($message);
    }

    throw new BlizzardApiLoginException('Unhandled response code');
  }

  /**
   * Set the access token used by the client.
   *
   * @param array $token
   *   An array containing the access token.
   */
  public function setAccessToken($token) {
    if (!isset($token['access_token']) || !isset($token['created'])) {
      throw new InvalidArgumentException('Invalid access token');
    }
    $this->token = $token;
    return $this;
  }
}

/**
 * Implements an OAuth 2.0 client using the Google API Client library.
 */
class BlizzardApiLoginGoogleClient extends BlizzardApiLoginClient {
  /**
   * Constructs a BlizzardApiLoginGoogleClient object.
   *
   * @param string $region
   *   A supported Battle.net login region.
   * @param array $scopes
   *   (optional) A list of scopes to request during the login attempt.
   */
  public function __construct($region, $scopes = array()) {
    parent::__construct($region, $scopes);
    
    $info = libraries_load('battlenet-google-client');
    if (!$info['loaded']) {
      throw new BlizzardApiLoginException('Unable to load library');
    }
    
    $this->client = $this->createClient();
    
    // Google_Client::prepareScopes() requires something to be set.
    if (empty($this->scopes)) {
      $this->scopes = array(' ');
    }
    
    $this->client->setScopes($this->scopes);
  }
  
  /**
   * Authenticate a user and retrieve an access token.
   *
   * @param string $code
   *   An authorization code that was returned to the redirect URL.
   * 
   * @throws Google_Auth_Exception
   */
  public function authenticate($code) {
    $this->client->authenticate($code);
  }
  
  /**
   * Initializes a new Google_Client object.
   *
   * @throws Google_Exception
   */
  protected function createClient() {
    $config = new BattleNet_Config($this->region);
    $config->setCacheClass('Google_Cache_Null');
    $client = new Google_Client($config);
    $client->setApplicationName(BLIZZARDAPI_LOGIN_UA);
    $client->setClientId(variable_get('blizzardapi_public_key', ''));
    $client->setClientSecret(variable_get('blizzardapi_private_key', ''));
    $client->setRedirectUri($this->redirectUrl);
    return $client;
  }
  
  /**
   * {@inheritdoc}
   */
  public function getAccessToken() {
    $token = $this->client->getAccessToken();
    return json_decode($token, TRUE);
  }
  
  /**
   * {@inheritdoc}
   */
  public function getAccessTokenScopes() {
    $token = $this->getAccessToken();
    if (!isset($token['scope'])) {
      return array();
    }
    return explode(' ', $token['scope']);
  }
  
  /**
   * {@inheritdoc}
   */
  public function getAccountId() {
    $service = $this->getService();
    if ($service) {
      $data = array();
      try {
        $data = $service->account->getAccountId();
      }
      catch (Exception $e) {
        // Since this is a separate request, log any exceptions that occur.
        watchdog_exception('blizzardapi_login', $e);
      }
      if (isset($data['id'])) {
        return $data['id'];
      }
    }
    return FALSE;
  }

  /**
   * {@inheritdoc}
   */
  public function getAuthorizeUrl($state = '') {
    if (!empty($state)) {
      $this->client->setState($state);
    }
    return $this->client->createAuthUrl();
  }
  
  /**
   * {@inheritdoc}
   */
  public function getClient() {
    return $this->client;
  }
  
  /**
   * Get a service resource based on the client's current access token.
   * 
   * @return object
   *   A BattleNet_Service_Account object, or NULL if the client does not have
   *   a stored access token.
   */
  public function getService() {
    $token = $this->getAccessToken();
    if (!empty($token)) {
      return new BattleNet_Service_Account($this->client);
    }
    return NULL;
  }

  /**
   * Get a service resource using a Drupal user's cached access token (this
   * will also update the access token used by the client).
   *
   * @param object $user
   *   A Drupal user, preferably with a cached access token.
   *
   * @return object
   *   A BattleNet_Service_Account object, or NULL if the user did not have a
   *   stored access token.
   */
  public function getServiceForUser($user) {
    if (isset($user->uid) && $user->uid != 0) {
      $token = blizzardapi_login_user_token($user);
      if ($token) {
        $this->client->setAccessToken(json_encode($token));
        return new BattleNet_Service_Account($this->client);
      }
    }
    return NULL;
  }
  
  /**
   * {@inheritdoc}
   */
  public function isAccessTokenExpired() {
    return $this->client->isAccessTokenExpired();
  }
}

/**
 * -----------------------------------------------------------------------------
 * Exceptions
 * -----------------------------------------------------------------------------
 */

/**
 * Exception thrown when a generic error occurs during the OAuth 2.0 login
 * process.
 */
class BlizzardApiLoginException extends Exception {
  public function __construct($message = '', $code = 0, $previous = NULL) {
    // Since PHP 5.3.0
    if (method_exists($this, 'getPrevious')) {
      parent::__construct($message, $code, $previous);
    }
    else {
      parent::__construct($message, $code);
    }
  }
}

/**
 * Exception thrown when an error occurs during the authentication process.
 */
class BlizzardApiLoginAuthException extends BlizzardApiLoginException {}
