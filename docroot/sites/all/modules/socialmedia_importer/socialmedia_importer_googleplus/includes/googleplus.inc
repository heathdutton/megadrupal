<?php

/**
 * @file
 * Googleplus Application handler Class
 */

class SocialMediaImporterGoogleplusApplication extends SocialMediaImporterApplication {
  const SMI_GP_OAUTH2_REVOKE_URI = 'https://accounts.google.com/o/oauth2/revoke';
  const SMI_GP_OAUTH2_TOKEN_URI = 'https://accounts.google.com/o/oauth2/token';
  const SMI_GP_OAUTH2_AUTH_URL = 'https://accounts.google.com/o/oauth2/auth';
  const SMI_GP_OAUTH2_FEDERATED_SIGNON_CERTS_URL = 'https://www.googleapis.com/oauth2/v1/certs';
  const SMI_GP_DATA_URI = 'https://www.googleapis.com/plus/v1/people/me/activities/public';
  const SMI_GP_CLOCK_SKEW_SECS = 300;
  const SMI_GP_OAUTH2_TOKEN_LIFETIME_SECS = 300;
  // One day in seconds.
  const SMI_GP_MAX_TOKEN_LIFETIME_SECS = 86400;
  public $accessType = 'offline';
  public $approvalPrompt = 'force';
  public $state;

  /**
   * Init Googleplus Application.
   */
  public function __construct($id = NULL, $app_id = NULL, $app_secret = NULL) {
    parent::__construct($id, $app_id, $app_secret);
  }

  /**
   * Get the Dialog Url to ask the user for Permission.
   * 
   * @param array $scopes
   *   Set of permissions the Application requests.
   * 
   * @ return string
   */
  public function getAuthenticationUrl($scopes) {
    $authentication_url = NULL;
    $_SESSION['gp_state'] = md5(rand());

    $query_params = array(
      'response_type' => 'code',
      'state' => $_SESSION['gp_state'],
      'redirect_uri' => $this->redirectUrl,
      'client_id' => $this->appId,
      'scope' => $scopes,
      'access_type' => $this->accessType,
      'approval_prompt' => $this->approvalPrompt,
    );

    $authentication_url = self::SMI_GP_OAUTH2_AUTH_URL . '?' . http_build_query($query_params, '', '&');
    return $authentication_url;
  }

  /**
   * Get Access Token.
   * 
   * @param string $code
   *   The code from googleplus.
   *  
   * @return array
   *   Contains Access Token Data: access_token, refresh_token, id_token,
   *   expires_in and token_type.
   */
  public function getAccessToken($code = NULL) {
    $access_token_data = NULL;
    $response_code = 0;
    $response = NULL;
    if (!$code && array_key_exists('code', $_REQUEST)) {
      $code = $_REQUEST['code'];
    }

    if ($code) {
      $query_params = array(
        'code=' . $code,
        'grant_type=authorization_code',
        'redirect_uri=' . $this->redirectUrl,
        'client_id=' . $this->appId,
        'client_secret=' . $this->appSecret,
      );

      $query_string = 'code=' . $code . '&grant_type=authorization_code&redirect_uri=' . $this->redirectUrl;
      $query_string .= '&client_id=' . $this->appId . '&client_secret=' . $this->appSecret;

      $request_params = array(
        'headers' => array(
          'Content-Type' => 'application/x-www-form-urlencoded',
        ),
        'method' => 'POST',
        'data' => implode('&', $query_params),
      );
      $request_url = self::SMI_GP_OAUTH2_TOKEN_URI . '?';
      $response = drupal_http_request($request_url, $request_params);
      if (is_object($response)) {
        if (property_exists($response, 'code')) {
          $response_code = $response->code;
          if ($response_code == 200) {
            $access_token_data = json_decode($response->data);
          }
        }
      }
    }
    return $access_token_data;
  }

  /**
   * Authenticate Application.
   */
  public function authorizeApplication() {
    $access_token_data = NULL;
    $authorization_succeed = FALSE;
    $access_token = NULL;
    $refresh_token = NULL;
    $scopes = 'email';

    if (empty($_REQUEST) || !array_key_exists('code', $_REQUEST)) {
      $authentication_url = $this->getAuthenticationUrl($scopes);
      drupal_goto($authentication_url);
    }
    else {
      if (array_key_exists('code', $_REQUEST)) {
        $code = $_REQUEST['code'];
        if ($_SESSION['gp_state'] && (strcmp($_SESSION['gp_state'], $_REQUEST['state']) == 0)) {
          $access_token_data = $this->getAccessToken();
          if (is_object($access_token_data) &&
             (property_exists($access_token_data, 'access_token')) &&
             (property_exists($access_token_data, 'refresh_token'))) {
            $access_token = $access_token_data->access_token;
            $refresh_token = $access_token_data->refresh_token;
            if (!is_null($access_token) && (!is_null($refresh_token))) {
              $data = array(
                'access_token' => $access_token,
                'refresh_token' => $refresh_token,
                'is_authorized' => 1,
                'id' => $this->id,
              );
              $authorization_succeed = TRUE;
              unset($_SESSION['gp_state']);
              socialmedia_importer_application_save($data);
              drupal_goto(SOCIALMEDIA_IMPORTER_APP_LIST_PATH);
              drupal_set_message(t('The Application is authenticated. This Application can be use to get %provider data.',
              array('%provider' => $this->provider)));
            }
            else {
              drupal_set_message(t('Can not get access token from %provider. Check your Application Settings',
              array('%provider' => $this->provider)), 'error');
              drupal_goto(SOCIALMEDIA_IMPORTER_APP_LIST_PATH);
            }
          }
          else {
            drupal_set_message(t('Can not get access token from %provider. Check your Application Settings',
            array('%provider' => $this->provider)), 'error');
            drupal_goto(SOCIALMEDIA_IMPORTER_APP_LIST_PATH);
          }
        }
        else {
          drupal_set_message(t('The state does not match. You may be a victim of CSRF.'), 'error');
          drupal_goto(SOCIALMEDIA_IMPORTER_APP_LIST_PATH);
        }
      }
      else {
        drupal_set_message(t('No code from %provider. Check your Application Settings',
        array('%provider' => $this->provider)), 'error');
        drupal_goto(SOCIALMEDIA_IMPORTER_APP_LIST_PATH);
      }
    }
  }


  /**
   * Get Application Data.
   * 
   * @return string
   *   A JSON encoded string of the user activities.
   */
  public function getApplicationData() {
    if ($this->isAuthorized) {
      $access_token = $this->accessToken;
      $response = drupal_http_request(self::SMI_GP_DATA_URI . '?access_token=' . $access_token);
      if (is_object($response) && ($response->code == 200)) {
        $response_data = $response->data;
      }
    }
    echo $response_data;
    return $response_data;
  }

  /**
   * Validate an Application.
   */
  public function validateApplication() {
    return TRUE;
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
    $access_token = $this->accessToken;
    $response = drupal_http_request(self::SMI_GP_OAUTH2_REVOKE_URI . '?token=' . $access_token);
    // The Access Token for the Application is revoked.
    krumo($response);
    if ($response->code == 200) {
      $is_deauthorized['success'] = TRUE;
    }
    else {
      $data = json_decode($response->data);
      $is_deauthorized['error_message'] = $data->error;
    }
    return $is_deauthorized;
  }

}
