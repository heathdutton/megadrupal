<?php
/**
 * @file
 * Twitter Application.
 */

class SocialMediaImporterTwitterApplication extends SocialMediaImporterApplication {
  const SMI_TWITTER_ACCESS_TOKEN_URL = 'https://api.twitter.com/oauth/access_token';
  const SMI_TWITTER_AUTHENTICATE_URL = 'https://api.twitter.com/oauth/authenticate';
  const SMI_TWITTER_AUTHORIZE_URL = 'https://api.twitter.com/oauth/authorize';
  const SMI_TWITTER_REQUEST_TOKEN_URL = 'https://api.twitter.com/oauth/request_token';
  const SMI_TWITTER_API_HOST = 'https://api.twitter.com/1.1/';
  const SMI_TWITTER_INVALIDE_TOKEN_URL = 'https://api.twitter.com/oauth2/invalidate_token';
  const SMI_TWITTER_OAUTH_HOST = 'https://api.twitter.com/oauth2/token';

  public $httpCode;
  public $url = '';
  public $timeout = 30;
  public $connecttimeout = 30;
  public $sslVerifypeer = FALSE;
  public $format = 'json';
  public $decodeJson = TRUE;
  public $httpInfo;
  public $useragent = 'TwitterOAuth v0.2.0-beta2';

  /**
   * Init Twitter Application.
   */
  public function __construct($id = NULL, $app_id = NULL, $app_secret = NULL) {
    parent::__construct($id, $app_id, $app_secret);
    $this->consumer = new DrupalOAuthConsumer($this->appId, $this->appSecret, $this->redirectUrl);
    if (!is_null($this->accessToken) && !is_null($this->accessTokenSecret)) {
      $this->token = new DrupalOAuthConsumer($this->accessToken, $this->accessTokenSecret);
    }
    else {
      $this->token = NULL;
    }
  }

  /**
   * Authorize Application.
   */
  public function authorizeApplication($id = NULL) {
    $authorization_succeed = FALSE;
    if ($id == NULL) {
      $id = $this->id;
      $_SESSION['twitter_app_id'] = $id;
    }
    elseif ($id) {
      $_SESSION['twitter_app_id'] = $id;
    }
    if ($id > 0) {
      if (empty($_REQUEST) || !isset($_REQUEST['oauth_token'])) {
        $token = $this->getRequestToken();
        if (is_array($token) && (count($token) > 0) &&
          array_key_exists('oauth_token', $token) &&
          array_key_exists('oauth_token_secret', $token)) {

          $_SESSION['twitter_oauth_token'] = $token['oauth_token'];
          $_SESSION['twitter_oauth_token_secret'] = $token['oauth_token_secret'];
          $request_url = $this->getAuthorizeURL($token['oauth_token']);
          $authorization_succeed = TRUE;
          drupal_goto($request_url);
        }
        else {
          $message = array_keys($token);
          drupal_set_message(t('%error_message', array('%error_message' => $message['0'])), 'error');
          drupal_goto(SOCIALMEDIA_IMPORTER_APP_LIST_PATH);
        }
      }
      else {
        if (isset($_REQUEST['oauth_token'])) {
          $access_token_data = $this->getAccessToken($_REQUEST['oauth_verifier']);
          if (is_array($access_token_data) &&
             (array_key_exists('oauth_token', $access_token_data)) &&
             (array_key_exists('oauth_token_secret', $access_token_data))) {

            unset($_SESSION['twitter_oauth_token']);
            unset($_SESSION['twitter_oauth_token_secret']);
            unset($_SESSION['twitter_app_id']);
            unset($_SESSION['smi_id']);
            $access_token = $access_token_data['oauth_token'];
            $access_token_secret = $access_token_data['oauth_token_secret'];
            if (!is_null($access_token) && (!is_null($access_token_secret))) {
              $data = array(
                'access_token' => $access_token,
                'access_token_secret' => $access_token_secret,
                'is_authorized' => 1,
                'id' => $this->id,
              );
              $authorization_succeed = TRUE;
              socialmedia_importer_application_save($data);
              drupal_goto(SOCIALMEDIA_IMPORTER_APP_LIST_PATH);
              drupal_set_message(t('The Application is authenticated. This Application can be use to get %provider data.',
              array('%provider' => $this->provider)));
            }
          }
        }
      }
      if (!$authorization_succeed) {
        // Can not get a valid token for the request.
        drupal_set_message(t('%error_message', array('%error_message' => self::NO_VALID_APP_ERROR)), 'error');
        drupal_goto(SOCIALMEDIA_IMPORTER_APP_LIST_PATH);
      }
    }
  }

  /**
   * Get Token for Request.
   */
  public function getRequestToken() {
    $request_parameters['oauth_callback'] = $this->redirectUrl;
    $request = $this->TwitterOauthRequest(self::SMI_TWITTER_REQUEST_TOKEN_URL, 'GET', $request_parameters);
    $token = OAuthUtil::parse_parameters($request);
    if (is_array($token) && count($token) > 0) {
      if (array_key_exists('oauth_token', $token) && array_key_exists('oauth_token_secret', $token)) {
        $this->token = new DrupalOAuthConsumer($token['oauth_token'], $token['oauth_token_secret']);
      }
    }
    return $token;
  }

  /**
   * Get Url for Permission Request.
   */
  public function getAuthorizeURL($token, $sign_in_with_twitter = TRUE) {
    if (is_array($token)) {
      $token = $token['oauth_token'];
    }
    if (empty($sign_in_with_twitter)) {
      return self::SMI_TWITTER_AUTHORIZE_URL . "?oauth_token=" . $token;
    }
    else {
      return self::SMI_TWITTER_AUTHENTICATE_URL . "?oauth_token=" . $token;
    }
  }

  /**
   * Get Access Token.
   */
  public function getAccessToken($oauth_verifier) {
    $this->token = new DrupalOAuthConsumer($_SESSION['twitter_oauth_token'], $_SESSION['twitter_oauth_token_secret']);
    $parameters = array();
    $parameters['oauth_verifier'] = $oauth_verifier;
    $response = $this->TwitterOauthRequest(self::SMI_TWITTER_ACCESS_TOKEN_URL, 'GET', $parameters);
    $token = OAuthUtil::parse_parameters($response);
    $this->token = new DrupalOAuthConsumer($token['oauth_token'], $token['oauth_token_secret']);
    return $token;
  }

  /**
   * Make Oauth Request.
   */
  public function TwitterOauthRequest($url, $method, $request_params) {
    $response = FALSE;
    if (strrpos($url, 'https://') !== 0 && strrpos($url, 'http://') !== 0) {
      $url = self::SMI_TWITTER_API_HOST . $url . "." . $this->format;
    }
    $request = OAuthRequest::from_consumer_and_token($this->consumer, $this->token, $method, $url, $request_params);
    $request->sign_request($this->sha1Method, $this->consumer, $this->token);
    $response = drupal_http_request($request->to_url());
    return $response->data;
  }

  /**
   * Get Application Data.
   */
  public function getApplicationData() {
    $consumer = new DrupalOAuthConsumer($this->appId, $this->appSecret);
    $token = new DrupalOAuthConsumer($this->accessToken, $this->accessTokenSecret);
    $method = 'GET';
    $url = 'statuses/user_timeline';
    $request_params = array();
    $response = $this->TwitterOauthRequest($url, $method, $request_params);
    echo $response;
    return $response;
  }

  /**
   * Validate a Twitter Application.
   *
   * @return bool
   *   Return TRUE if the Application is a valid Twitter Application.
   */
  public function validateApplication() {
    $app_id = $this->appId;
    $app_secret = $this->appSecret;
    if (!is_null($app_id) && !is_null($app_secret)) {
      $is_valid = TRUE;
      $authorization_token = urlencode($app_id) . ':' . urlencode($app_secret);
      $query_params = array(
        'headers' => array(
          'Accept-Encoding' => 'gzip',
          'Content-Type' => 'application/x-www-form-urlencoded;charset=UTF-8',
          'Authorization' => 'Basic ' . base64_encode($authorization_token),
        ),
        'data' => 'grant_type=client_credentials',
        'method' => 'POST',
      );
      $graf_url = 'https://api.twitter.com/oauth2/token?';
      $response = drupal_http_request($graf_url, $query_params);
      if (is_object($response) && property_exists($response, 'error')) {
        $is_valid = FALSE;
      }
    }
    else {
      $is_valid = FALSE;
    }
    return $is_valid;
  }

  /**
   * Get Application Only token.
   * 
   * @return string
   *   The Bearer Token
   */
  public function getBearerToken() {
    $bearertoken = NULL;
    if (!is_null($this->appId) && !is_null($this->appSecret)) {
      $authorization_token = urlencode($this->appId) . ':' . urlencode($this->appSecret);
      $query_params = array(
        'headers' => array(
          'Content-Type' => 'application/x-www-form-urlencoded;charset=UTF-8',
          'Authorization' => 'Basic ' . base64_encode($authorization_token),
        ),
        'data' => 'grant_type=client_credentials',
        'method' => 'POST',
      );
      $response = drupal_http_request(self::SMI_TWITTER_OAUTH_HOST, $query_params);
      if (is_object($response)) {
        if (property_exists($response, 'error')) {
          $bearertoken = NULL;
        }
        elseif (property_exists($response, 'data')) {
          $bearertokenobject = json_decode($response->data);
          $bearertoken = $bearertokenobject->access_token;
        }
      }
    }
    return $bearertoken;
  }

  /**
   * Deauthorize an Application.
   * 
   * @return array
   *   An associative array with keys:
   *   - success: TRUE or FALSE
   *   - (optional) error_message: The error message in case of error.
   */
  public function deauthorizeApplication() {
    $is_deauthorized = array(
      'success' => FALSE,
    );
    $authorization_token = urlencode($this->appId) . ':' . urlencode($this->appSecret);
    $query_params = array(
      'headers' => array(
        'Content-Type' => 'application/x-www-form-urlencoded;charset=UTF-8',
        'Authorization' => 'Basic ' . base64_encode($authorization_token),
      ),
      'method' => 'POST',
      'data' => 'access_token=' . $this->getBearerToken(),
    );
    $response = drupal_http_request(self::SMI_TWITTER_INVALIDE_TOKEN_URL, $query_params);
    if (is_object($response) && $response->code == '200') {
      $is_deauthorized['success'] = TRUE;
    }
    return $is_deauthorized;
  }

}
