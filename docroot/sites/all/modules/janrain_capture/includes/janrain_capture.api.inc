<?php

/**
 * @file
 * An API Client for making calls to the Janrain Capture web service.
 */

class JanrainCaptureApi {

  protected $args;
  protected $captureAddr;

  /**
   * Retrives API access credentials from settings.
   */
  function __construct() {
    $ver = variable_get('janrain_capture_ver', JANRAIN_CAPTURE_VERSION_DEFAULT);
    if($ver == '1.0') {
      $janrain_capture_main = variable_get('janrain_capture_main', array());
    } else {
      $janrain_capture_main = variable_get('janrain_capture_main2', array());
    }
      $this->args = array();
      $this->args['client_id'] = isset($janrain_capture_main['capture_client_id'])
          ? $janrain_capture_main['capture_client_id']
          : '';
      $this->args['client_secret'] = isset($janrain_capture_main['capture_client_secret'])
          ? $janrain_capture_main['capture_client_secret']
          : '';
      $this->captureAddr = !empty($janrain_capture_main['capture_address'])
        ? $janrain_capture_main['capture_address']
        : '';
      $this->captureAddr = str_replace("https://", "", $this->captureAddr);
  }

  /**
   * Performs the HTTP request.
   *
   * @param string $command
   *   The Capture command to perform
   * @param array $arg_array
   *   The data set to pass via POST
   * @param string $access_token
   *   The client access token to use when performing user-specific calls
   *
   * @return mixed
   *   The HTTP request result data
   */
  protected function call($command, $arg_array = NULL, $access_token = NULL) {

    $url = "https://" . $this->captureAddr . "/$command";

    $headers = array(
      'Content-Type' => 'application/x-www-form-urlencoded',
      'User-Agent' => 'Drupal/janrain_capture-' . JANRAIN_CAPTURE_MODULE_VERSION,
    );

    if (isset($access_token)) {
      $headers['Authorization'] = "OAuth $access_token";
    }
    $options = array('headers' => $headers);

    if ($arg_array) {
      $arg_array = array_merge($arg_array, $this->args);
    }
    else {
      $arg_array = $this->args;
    }

    $options['method'] = 'POST';
    $options['data'] = http_build_query($arg_array, '', '&');
    $result = drupal_http_request($url, $options);
    $json_data = $this->decodeData($result);

    // NULL decoded value indicates a parse error.
    if (!isset($json_data)) {
      $json_data['stat'] = 'error';
      $json_data['code'] = '0';
      $json_data['error'] = 'JSON parse error for: ' . $result->data;
    }

    if ($json_data['stat'] == 'error') {
      $error = new stdClass();
      $error->code = $json_data['code'];
      $error->error = $json_data['error'] . ": ". $json_data['error_description'];
      $this->reportError($error);
      return FALSE;
    }

    return $json_data;
  }

  protected function decodeData($result) {
    if (!isset($result->data) || $result->code != '200') {
      $this->reportError($result);
      return FALSE;
    }

    $json_data = json_decode($result->data, TRUE);

    return $json_data;
  }

  /**
   * Updates session variables with Capture user tokens
   *
   * @param string $json_data
   *   The data received from the HTTP request containing the tokens
   */
  protected function updateCaptureSession($json_data) {
    $_SESSION['janrain_capture_access_token'] = $json_data['access_token'];
    $_SESSION['janrain_capture_refresh_token'] = $json_data['refresh_token'];
    $_SESSION['janrain_capture_expires_in'] = REQUEST_TIME + $json_data['expires_in'];

    $password_recover = (isset($json_data['transaction_state']['capture']['password_recover'])
        && $json_data['transaction_state']['capture']['password_recover'] == TRUE) ? TRUE : FALSE;
    $_SESSION['janrain_capture_password_recover'] = $password_recover;
    if (isset($json_data['transaction_state']['capture']['action'])) {
      $_SESSION['janrain_capture_action'] = $json_data['transaction_state']['capture']['action'];
    }
  }

  /**
   * Helper function for the Engage web API wrappers.
   *
   * @param stdClass $result
   *   Result containing error code and message
   */
  protected function reportError($result) {
    watchdog('janrain_capture', 'Capture web API seems to be inaccessible due to "%error".', array('%error' => $result->code . ' - ' . $result->error), WATCHDOG_WARNING);
    drupal_set_message(t('Capture web API seems to be inaccessible because of error "%error".', array('%error' => $result->code . ' - ' . $result->error)), 'error');
  }

  /**
   * Perform the exchange to generate a new Access Token
   *
   * @param string $auth_code
   *   The authorization token to use for the exchange
   * @param string $redirect_uri
   *   The redirect_uri string to match for the exchange
   */
  public function newAccessToken($auth_code, $redirect_uri) {
    $command = "oauth/token";
    $arg_array = array(
      'code' => $auth_code,
      'redirect_uri' => $redirect_uri,
      'grant_type' => 'authorization_code',
    );

    $json_data = $this->call($command, $arg_array);
    if ($json_data) {
      $this->updateCaptureSession($json_data);
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Retrieves a new access_token/refresh_token set
   *
   * @return boolean
   *   Boolean success or failure
   */
  function refreshAccessToken() {
    if (empty($_SESSION['janrain_capture_refresh_token'])) {
      return FALSE;
    }

    $refresh_token = $_SESSION['janrain_capture_refresh_token'];
    $command = "oauth/token";
    $arg_array = array(
      'refresh_token' => $refresh_token,
      'grant_type' => 'refresh_token',
    );

    $json_data = $this->call($command, $arg_array);

    if ($json_data) {
      $this->updateCaptureSession($json_data);
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Retrives the user entity from Capture
   *
   * @param boolean $can_refresh
   *   Allow this function to refresh the token set if needed
   *
   * @return mixed
   *   The entity retrieved or null
   */
  public function loadUserEntity($can_refresh = TRUE) {
    if (empty($_SESSION['janrain_capture_access_token'])) {
      return NULL;
    }
    $user_entity = NULL;

    $need_to_refresh = FALSE;

    // Check if we need to refresh the access token.
    if (REQUEST_TIME >= $_SESSION['janrain_capture_expires_in']) {
      $need_to_refresh = TRUE;
    }
    else {
      $user_entity = $this->call('entity', array(), $_SESSION['janrain_capture_access_token']);
      if (isset($user_entity['code']) && $user_entity['code'] == '414') {
        $need_to_refresh = TRUE;
      }
    }

    // If necessary, refresh the access token and try to fetch the entity again.
    if ($need_to_refresh && $can_refresh) {
      if ($this->refreshAccessToken()) {
        $user_entity = $this->loadUserEntity(FALSE);
      }
    }

    // Return NULL if there is an error code.
    return isset($user_entity['code']) ? NULL : $user_entity;
  }

  /**
   * Retrieves a list of Social Sharing providers for Engage
   */
  public function getShareProviders() {
    $items = $this->call('settings/items');
    
    $rpx_key = $items['result']['rpx_key'];

    $headers = array(
      'Content-Type' => 'application/x-www-form-urlencoded',
      'User-Agent' => 'Drupal/janrain_capture-' . JANRAIN_CAPTURE_MODULE_VERSION,
    );

    $url = sprintf('https://rpxnow.com/plugin/lookup_rp?apiKey=%s', $rpx_key);
    $options = array('headers' => $headers);
    $result = drupal_http_request($url, $options);

    $json_data = $this->decodeData($result);

    if (isset($json_data['shareProviders'])) {
      return explode(',', $json_data['shareProviders']);
    }
    else {
      return FALSE;
    }
  }

}
