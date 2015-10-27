<?php

/**
 * Exception handling class.
 */
class D500pxException extends Exception {}



/**
 * Primary 500px API implementation class
 */
class D500px {
  /**
   * @var $source the 500px api 'source'
   */
  protected $source = 'drupal';
  protected $signature_method;
  protected $consumer;
  protected $token;


  /********************************************//**
   * Authentication
   ***********************************************/
  /**
   * Constructor for the 500px class
   */
  public function __construct($consumer_key, $consumer_secret, $oauth_token = NULL, $oauth_token_secret = NULL) {
    $this->signature_method = new OAuthSignatureMethod_HMAC_SHA1();
    $this->consumer = new OAuthConsumer($consumer_key, $consumer_secret);
    if (!empty($oauth_token) && !empty($oauth_token_secret)) {
      $this->token = new OAuthConsumer($oauth_token, $oauth_token_secret);
    }
  }


  public function get_request_token() {
    $url = variable_get('d500px_api', D500PX_API) . '/v1/oauth/request_token';
    try {
      $params = array('oauth_callback' => url('d500px/oauth', array('absolute' => TRUE)));
      $response = $this->auth_request($url, $params);
    }
    catch (D500pxException $e) {
      watchdog('D500px', '!message', array('!message' => $e->__toString()), WATCHDOG_ERROR);
      return FALSE;
    }
    parse_str($response, $token);
    $this->token = new OAuthConsumer($token['oauth_token'], $token['oauth_token_secret']);
    return $token;
  }


  public function get_authorize_url($token) {
    $url = variable_get('d500px_api', D500PX_API) . '/v1/oauth/authorize';
    $url.= '?oauth_token=' . $token['oauth_token'];

    return $url;
  }  
  
  
  public function get_authenticate_url($token) {
    $url = variable_get('d500px_api', D500PX_API) . '/v1/oauth/authenticate';
    $url.= '?oauth_token=' . $token['oauth_token'];

    return $url;
  }  
  
  
  public function get_access_token() {
    $url = variable_get('d500px_api', D500PX_API) . '/v1/oauth/access_token';
    try {
      $response = $this->auth_request($url);
    }
    catch (D500pxException $e) {
      watchdog('D500px', '!message', array('!message' => $e->__toString()), WATCHDOG_ERROR);
      return FALSE;
    }
    parse_str($response, $token);
    $this->token = new OAuthConsumer($token['oauth_token'], $token['oauth_token_secret']);
    return $token;
  } 
  
  
  /**
   * Performs an authenticated request.
   */
  public function auth_request($url, $params = array(), $method = 'GET') {
    $request = OAuthRequest::from_consumer_and_token($this->consumer, $this->token, $method, $url, $params);
    $request->sign_request($this->signature_method, $this->consumer, $this->token);
    switch ($method) {
      case 'GET':
        return $this->request($request->to_url());
      case 'POST':
        return $this->request($request->get_normalized_http_url(), $request->get_parameters(), 'POST');
    }
  }
  
  
  /**
   * Performs a request.
   *
   * @throws D500pxException
   */
  protected function request($url, $params = array(), $method = 'GET') {
    $data = '';
    if (count($params) > 0) {
      if ($method == 'GET') {
        $url .= '?'. http_build_query($params, '', '&');
      }
      else {
        $data = http_build_query($params, '', '&');
      }
    }

    $headers = array();
    $headers['Authorization'] = 'Oauth';
    $headers['Content-type'] = 'application/x-www-form-urlencoded';
    $response = $this->doRequest($url, $headers, $method, $data);
    
    if (!isset($response->error)) {
      return $response->data;
    }
    else {
      $error = $response->error;
      $data = $this->parse_response($response->data);
      if (isset($data['error'])) {
        $error = $data['error'];
      }
      throw new D500pxException($error);
    }
  }
  
  
  /**
   * Actually performs a request.
   *
   * This method can be easily overriden through inheritance.
   *
   * @param string $url
   *   The url of the endpoint.
   * @param array $headers
   *   Array of headers.
   * @param string $method
   *   The HTTP method to use (normally POST or GET).
   * @param array $data
   *   An array of parameters
   * @return
   *   stdClass response object.
   */
  protected function doRequest($url, $headers, $method, $data) {
    return drupal_http_request($url, array('headers' => $headers, 'method' => $method, 'data' => $data));
  }  


  protected function parse_response($response) {
    // http://drupal.org/node/985544 - json_decode large integer issue
    $length = strlen(PHP_INT_MAX);
    $response = preg_replace('/"(id|in_reply_to_status_id)":(\d{' . $length . ',})/', '"\1":"\2"', $response);
    return json_decode($response, TRUE);
  }


  /**
   * Creates an API endpoint URL.
   *
   * @param string $path
   *   The path of the endpoint.
   * @return
   *   The complete path to the endpoint.
   */
  protected function create_url($path) {
    $url =  variable_get('d500px_api', D500PX_API) .'/v1/'. $path;
    return $url;
  }



  /********************************************//**
   * Utilities
   ***********************************************/
  /**
   * Calls a 500px API endpoint.
   */
  public function call($path, $params = array(), $method = 'GET') {
    $url = $this->create_url($path);

    try {
      $response = $this->auth_request($url, $params, $method);
    }
    catch (D500pxException $e) {
      watchdog('D500px', '!message', array('!message' => $e->__toString()), WATCHDOG_ERROR);
      return FALSE;
    }

    if (!$response) {
      return FALSE;
    }
    
    return $this->parse_response($response);
  }
       
}