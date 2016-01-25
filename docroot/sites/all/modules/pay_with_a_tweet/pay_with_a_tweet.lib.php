<?php

/**
 * @file
 * Class required to twitter access and publish tweet
 * for the Pay with a Tweet module.
 *
 * @ingroup pay_with_a_tweet
 */

/**
 * Pay with a Tweet OAuth class
 */
class PayWithATweetOAuth {

  /* Contains the access_token URL. */
  protected $accessTokenUrl = 'https://api.twitter.com/oauth/access_token';
  /* Contains the authenticate URL. */
  protected $authenticateUrl = 'https://api.twitter.com/oauth/authenticate';
  /* Contains the authorize URL. */
  protected $authorizeUrl = 'https://api.twitter.com/oauth/authorize';
  /* Contains the consumer. */
  protected $consumer;
  /* Contains the consumer key. */
  protected $consumerKey = NULL;
  /* Contains the consume key. */
  protected $consumerSecret = NULL;
  /* Contains the last request error. */
  protected $error = NULL;
  /* Contains up the API root URL. */
  protected $host = "https://api.twitter.com/1.1/";
  /* Contains the last request http code. */
  protected $httpCode = NULL;
  /* Contains the oauth token. */
  protected $oauthToken = NULL;
  /* Contains the oauth token secret. */
  protected $oauthTokenSecret = NULL;
  /* Contains the request_token URL. */
  protected $requestTokenUrl = 'https://api.twitter.com/oauth/request_token';
  /* Contains the signature method. */
  protected $signatureMethod = NULL;
  /* Contains the token value. */
  protected $token = NULL;

  /**
   * Construct PayWithATweetOAuth object.
   */
  public function __construct($consumer_key = NULL, $consumer_secret = NULL, $oauth_token = NULL, $oauth_token_secret = NULL) {

    if (!empty($consumer_key) && !empty($consumer_secret)) {
      $this->setSignatureMethod();
      $this->setConsumerKey($consumer_key);
      $this->setConsumerSecret($consumer_secret);
      $this->setConsumer();

      if (!empty($oauth_token) && !empty($oauth_token_secret)) {
        $this->setToken($oauth_token, $oauth_token_secret);
      }
    }
  }

  /**
   * Exchange request token&secret for an access token&secret,to sign API calls.
   *
   * @return array
   *   a key/value array containing oauth_token, oauth_token_secret,
   *   user_id and screen_name
   */
  public function getAccessToken($oauth_verifier = FALSE) {
    $parameters = array();
    if (!empty($oauth_verifier)) {
      $parameters['oauth_verifier'] = $oauth_verifier;
    }
    $request = $this->request($this->accessTokenUrl, $parameters);
    $token = OAuthUtil::parse_parameters($request);
    $this->setToken($token['oauth_token'], $token['oauth_token_secret']);
    return $token;
  }

  /**
   * Get the authorize URL.
   *
   * @returns a string
   */
  public function getAuthorizeURL($token = NULL, $sign_in_with_twitter = TRUE) {
    $query = NULL;

    if (!empty($token)) {
      if (is_string($token)) {
        $query = '?oauth_token=' . $token;
      }
      elseif (is_array($token) && array_key_exists($token['oauth_token'])) {
        $query = '?oauth_token=' . $token['oauth_token'];
      }
    }
    elseif (!empty($this->oauthToken)) {
      $query = '?oauth_token=' . $this->oauthToken;
    }

    if (empty($sign_in_with_twitter)) {
      return $this->authorizeUrl . $query;
    }
    else {
      return $this->authenticateUrl . $query;
    }
  }

  /**
   * Verify the user credentials in Twitter.
   * 
   * @return object
   *   object with the twitter user info.
   */
  public function getCredentials() {

    $return = FALSE;

    $url = $this->host . 'account/verify_credentials.json';
    return json_decode($this->request($url));
  }

  /**
   * Get the last request error.
   * 
   * @return string
   *   the last request error value
   */
  public function getError() {
    return $this->error;
  }

  /**
   * Get the last request HTTP Code.
   * 
   * @return string
   *   the last request HTTP Code
   */
  public function getHttpCode() {
    return $this->httpCode;
  }

  /**
   * Get a request_token from Twitter.
   *
   * @return array
   *   a key/value array containing oauth_token and oauth_token_secret
   */
  public function getRequestToken($oauth_callback = NULL) {
    $parameters = array();
    if (!empty($oauth_callback)) {
      $parameters['oauth_callback'] = $oauth_callback;
    }
    $request = $this->request($this->requestTokenUrl, $parameters);
    $token = OAuthUtil::parse_parameters($request);
    $this->setToken($token['oauth_token'], $token['oauth_token_secret']);
    return $token;
  }

  /**
   * Publish the tweet at the user account.
   * 
   * @param string $message
   *   The content of the tweet.
   * 
   * @return object
   *   the tweet published.
   */
  public function publishTweet($message) {
    $url = $this->host . 'statuses/update.json';
    $parameters = array('status' => $message);

    return json_decode($this->request($url, $parameters, 'POST'));
  }

  /**
   * Send an HTTP request to twitter.
   *
   * @param string $url
   *   The url to call.
   * @param array $parameters
   *   The params to send.
   * @param string $method
   *   The method to send the parameters (GET or POST).
   *
   * @return string
   *   the data response.
   */
  protected function request($url, $parameters = array(), $method = 'GET') {

    $return = NULL;

    $request = OAuthRequest::from_consumer_and_token($this->consumer, $this->token, $method, $url, $parameters);
    $request->sign_request($this->signatureMethod, $this->consumer, $this->token);

    $headers = array(
      'Authorization' => 'Oauth',
      'Content-type' => 'application/x-www-form-urlencoded',
    );

    $options = array(
      'headers' => $headers,
      'method' => $method,
      'data' => $method == 'POST' ? http_build_query($request->get_parameters(), '', '&') : '',
    );

    if ($method == 'GET') {
      $response = drupal_http_request($request->to_url(), $options);
    }
    else {
      $response = drupal_http_request($request->get_normalized_http_url(), $options);
    }

    $this->setHttpCode($response->code);
    if (!isset($response->error)) {
      $return = $response->data;
      $this->setError($response->error . '<hr />' . $response->data);
    }
    else {
      $this->setError(NULL);
    }

    return $return;
  }

  /**
   * Set the consumer (OAuthConsumer object).
   *
   * @param string $consumer_key
   *   The consumer key.
   * @param string $consumer_secret
   *   The consumer secret.
   */
  public function setConsumer($consumer_key = NULL, $consumer_secret = NULL) {
    if (!empty($consumer_key)) {
      $this->setConsumerKey($consumer_key);
    }

    if (!empty($consumer_secret)) {
      $this->setConsumerSecret($consumer_secret);
    }

    $this->consumer = new OAuthConsumer($this->consumerKey, $this->consumerSecret);
  }

  /**
   * Set the consumerKey.
   * 
   * @param string $consumer_key
   *   the new consumerKey.
   */
  public function setConsumerKey($consumer_key) {
    $this->consumerKey = $consumer_key;
  }

  /**
   * Set the consumerSecret.
   * 
   * @param string $consumer_secret
   *   the new consumerSecret.
   */
  public function setConsumerSecret($consumer_secret) {
    $this->consumerSecret = $consumer_secret;
  }

  /**
   * Set the last request error.
   * 
   * @param string $error
   *   the new error value.
   */
  protected function setError($error) {
    $this->error = $error;
  }

  /**
   * Set the last request HTTP code.
   * 
   * @param string $code
   *   the new request HTTP code.
   */
  protected function setHttpCode($code) {
    $this->httpCode = $code;
  }

  /**
   * Set the oauthToken value.
   * 
   * @param string $oauth_token
   *   the new oauthToken value.
   */
  public function setOauthToken($oauth_token = NULL) {
    if (!empty($oauth_token)) {
      $this->oauthToken = $oauth_token;
    }
  }

  /**
   * Set the oauthTokenSecret value.
   * 
   * @param string $oauth_token_secret
   *   the new oauthTokenSecret value.
   */
  public function setOauthTokenSecret($oauth_token_secret) {
    if (!empty($oauth_token_secret)) {
      $this->oauthTokenSecret = $oauth_token_secret;
    }
  }

  /**
   * Set and start the signatureMethod.
   * 
   * @param string $class_name
   *   The class to instance the signatureMethod object.
   */
  public function setSignatureMethod($class_name = NULL) {
    if (!empty($class_name) && class_exists($class_name)) {
      $this->signatureMethod = new $class_name();
    }
    else {
      $this->signatureMethod = new OAuthSignatureMethod_HMAC_SHA1();
    }
  }

  /**
   * Set the token value (OAuthConsumer object).
   * 
   * @param string $oauth_token
   *   Optional. The new oauth_token to use.
   * @param string $oauth_token_secret
   *   Optional. The new oauth_token_secret to use.
   */
  public function setToken($oauth_token = NULL, $oauth_token_secret = NULL) {
    if (!empty($oauth_token)) {
      $this->setOauthToken($oauth_token);
    }

    if (!empty($oauth_token_secret)) {
      $this->setOauthTokenSecret($oauth_token_secret);
    }

    $this->token = new OAuthConsumer($this->oauthToken, $this->oauthTokenSecret);
  }

}
