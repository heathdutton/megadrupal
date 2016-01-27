<?php
/**
 * @file
 * Implements Authy authentication. It's like magic, except it's math.
 */

class Authy {

  public $_apiKey  = NULL;

  public $_serverTimeout = 3;

  public $lastError = NULL;

  public $_server = 'https://api.authy.com';

  /**
   * Add a new Authy user and get the Authy ID.
   *
   * @param string $email
   *   User email.
   *
   * @param string $cellphone
   *   User cellphone.
   *
   * @param string $countrycode
   *   User countrycode. Defaults to 1 (USA).
   *
   * @return mixed
   *   Returns the users Authy ID on success or FALSE on error.
   */
  public function userNew($email, $cellphone, $countrycode = 1) {

    $data = array(
      'user[email]'        => $email,
      'user[cellphone]'    => $cellphone,
      'user[country_code]' => $countrycode,
    );

    $result = $this->apiCall('new', $data);

    if($result === FALSE) {
      $this->lastError = 'AUTHY_SERVER_ERROR';
      return FALSE;
    }

    if (isset($result->errors)) {
      if (isset($result->errors->api_key)) {
        $this->lastError = 'AUTHY_SERVER_INVALID_API_KEY';
      } else {
        $this->lastError = 'AUTHY_SERVER_INVALID_DATA';
      }
      return FALSE;
    }

    if (isset($result->user->id)) {
      return $result->user->id;
    }
    $this->lastError = 'AUTHY_SERVER_SAYS_NO';
    return FALSE;
  }

  /**
   * Verify a Authy OTP.
   *
   * @param int $authyId
   *   User Authy ID.
   *
   * @param int $token
   *   User supplied OPT/token.
   *
   * @return boolean
   *   Return true if a valid Authy token is supplied, false on any errors.
   */
  public function verify($authyId, $token) {
    $data = array(
      'token'    => $token,
      'authy_id' => $authyId,
    );

    $result = $this->apiCall('verify', $data);

    if ($result === FALSE) {
      $this->lastError = 'AUTHY_SERVER_ERROR';
      return FALSE;
    }

    if (isset($result->errors)) {
      if (isset($result->errors->message) && $result->errors->message == 'token is invalid') {
        $this->lastError = 'AUTHY_SERVER_BAD_OTP';
      }
      elseif (isset($result->errors->api_key)) {
        $this->lastError = 'AUTHY_SERVER_INVALID_API_KEY';
      }
      else {
        $this->lastError = 'AUTHY_SERVER_INVALID_DATA';
      }
      return FALSE;
    }

    if (isset($result->token) && $result->token == 'is valid') {
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Request SMS token.
   *
   * @param int $authyId
   *   Authy ID to request SMS token for.
   *
   * @return boolean
   *   Returns TRUE if SMS request was OK. False if not.
   */
  public function requestSms($authyId) {
    $data = array(
      'authy_id' => $authyId,
    );

    $result = $this->apiCall('sms', $data);

    if ($result === FALSE) {
      $this->lastError = 'AUTHY_SERVER_ERROR';
      return FALSE;
    }

    if (isset($result->errors)) {
      $this->lastError = 'AUTHY_SERVER_INVALID_DATA';
      return FALSE;
    }

    return TRUE;
  }

  /**
   * Performs a call to the Authy API.
   *
   * @param string $action
   *   Action to perform: should be 'new' or 'verify'
   *
   * @param array $data
   *   Data to perform the action with.
   *
   * @return mixed
   *   Decoded JSON results from Authy server or FALSE on error.
   */
  private function apiCall($action, $data) {

    switch ($action) {
      case 'new':
        $url = $this->_server.'/protected/json/users/new?api_key='.$this->_apiKey;
        $postData = http_build_query($data);
        $opts =  array(
          'method'  => 'POST',
          'timeout' => $this->_serverTimeout,
          'data' => $postData,
        );
        break;

      case 'verify':
        $url = $this->_server.'/protected/json/verify/'.$data['token'].'/'.$data['authy_id'].'?api_key='.$this->_apiKey.'&force=true';

        $opts = array(
          'method'  => 'GET',
          'timeout' => $this->_serverTimeout,
        );
        break;

      case 'sms':
        $url = $this->_server.'/protected/json/sms/'.$data['authy_id'].'?api_key='.$this->_apiKey.'&force=true';

        $opts = array(
          'method'  => 'GET',
          'timeout' => $this->_serverTimeout,
        );
        break;
    }

    $request = drupal_http_request($url, $opts);

    if (!isset($request->data)) {
      return FALSE;
    }

    return json_decode($request->data);
   }
}
