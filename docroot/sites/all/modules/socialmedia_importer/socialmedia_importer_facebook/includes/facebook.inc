<?php

/**
 * @file
 * Facebook Application Class.
 */

class SocialMediaImporterFacebookApplication extends SocialMediaImporterApplication {
  const SMI_FB_OAUTH_BASE_URL = 'https://graph.facebook.com/oauth/access_token?';
  const SMI_FB_TOKEN_DEBUG_BASE_URL = 'https://graph.facebook.com/debug_token?';
  const SMI_FB_DIALOG_BASE_URL = 'https://www.facebook.com/dialog/oauth?';

  /**
   * Get the Dialog Url to ask the user for Permission.
   */
  public function getDialogUrl($scopes) {
    // CSRF protection.
    $_SESSION['state'] = md5(uniqid(rand(), TRUE));
    if (empty($scopes)) {
      $scopes = array('read_stream');
    }
    $query_params = array(
      'client_id' => $this->appId,
      'state' => $_SESSION['state'],
      'redirect_uri' => $this->redirectUrl,
      'scope' => implode(',', $scopes),
    );
    $dialog_url = self::SMI_FB_DIALOG_BASE_URL . http_build_query($query_params, '', '&');
    return $dialog_url;
  }

  /**
   * Get the Url for Token Request.
   */
  public function getTokenUrl($code) {
    if (strlen($code) <= 0) {
      if (array_key_exists('code', $_REQUEST)) {
        $code = $_REQUEST['code'];
      }
    }

    $query_params = array(
      'client_id' => $this->appId,
      'client_secret' => $this->appSecret,
      'redirect_uri' => $this->redirectUrl,
      'code' => $code,
    );
    $token_url = self::SMI_FB_OAUTH_BASE_URL . http_build_query($query_params, '', '&');
    return $token_url;
  }

  /**
   * Get the Access Token.
   */
  public function getAccessToken($token_url) {
    $access_token = NULL;
    $access_token_response = file_get_contents($token_url);
    if ($access_token_response !== FALSE) {
      parse_str($access_token_response, $response_params);
      if (array_key_exists('access_token', $response_params)) {
        $access_token = $response_params['access_token'];
      }
    }
    return $access_token;
  }

  /**
   * Autorize an Application.
   */
  public function authorizeApplication($id = NULL) {
    $authorization_succeed = FALSE;
    $code = NULL;
    $access_token = NULL;
    if ($id == NULL) {
      $id = $this->id;
      $_SESSION['facebook_app_id'] = $id;
    }
    elseif ($id) {
      $_SESSION['facebook_app_id'] = $id;
    }
    if ($id) {
      if (empty($_REQUEST) || !array_key_exists('code', $_REQUEST)) {
        $scopes = array('user_birthday', 'read_stream');
        $dialog_url = $this->getDialogUrl($scopes);
        drupal_goto($dialog_url);
      }
      else {
        if (array_key_exists('code', $_REQUEST)) {
          $code = $_REQUEST['code'];
          if ($_SESSION['state'] && (strcmp($_SESSION['state'], $_REQUEST['state']) == 0)) {
            if (!is_null($code)) {
              $token_url = $this->getTokenUrl($code);
              $access_token = $this->getAccessToken($token_url);
            }
            if (!is_null($access_token)) {
              $application_data = array(
                'id' => $this->id,
                'access_token' => $access_token,
                'is_authorized' => 1);
              $saved = socialmedia_importer_application_save($application_data);
              unset($_SESSION['facebook_app_id']);
              unset($_SESSION['state']);
              $authorization_succeed = TRUE;
              drupal_set_message(t('The Application is authenticated. This Application can be use to get %provider data.',
              array('%provider' => $this->provider)));
              drupal_goto(SOCIALMEDIA_IMPORTER_APP_LIST_PATH);
            }
            else {
              drupal_set_message(t('Can not get access token from Facebook. Check your Application Settings'), 'error');
              drupal_goto(SOCIALMEDIA_IMPORTER_APP_LIST_PATH);
            }
          }
          else {
            drupal_set_message(t('The state does not match. You may be a victim of CSRF.'), 'error');
            drupal_goto(SOCIALMEDIA_IMPORTER_APP_LIST_PATH);
          }
        }
        else {
          drupal_set_message(t('No code from Facebook. Check your Application Settings'), 'error');
          drupal_goto(SOCIALMEDIA_IMPORTER_APP_LIST_PATH);
        }
      }
      if (!$authorization_succeed) {
        unset($_SESSION['facebook_app_id']);
        drupal_set_message(t('The Application can not be authenticated. Check your Application Settings'), 'error');
        drupal_goto(SOCIALMEDIA_IMPORTER_APP_LIST_PATH);
      }
    }
  }

  /**
   * Extend an Application Access Token.
   */
  public function extendAccessToken() {
    $access_token = NULL;
    if (!is_null($this->accessToken)) {
      $response_params = array();
      $query_params = array(
        'client_id' => $this->appId,
        'client_secret' => $this->appSecret,
        'grant_type' => 'fb_exchange_token',
        'fb_exchange_token' => $this->accessToken,
        'redirect_uri' => $this->redirectUrl,
      );
      $request_url = self::SMI_FB_OAUTH_BASE_URL . http_build_query($query_params, '', '&');
      $response = drupal_http_request($request_url);
      if (is_object($response)) {
        parse_str($response->data, $response_params);
        if (array_key_exists('access_token', $response_params)) {
          $access_token = $response_params['access_token'];
        }
      }
    }
    return $access_token;
  }

  /**
   * Validate an Application Access Token.
   */
  public function validateAccessToken() {
    $response_params = array();
    $is_valid = FALSE;
    $response = NULL;
    $app_access_token = $this->getApplicationAccessToken();
    $query_params = array(
      'access_token' => $app_access_token,
      'input_token' => $this->accessToken,
    );
    if (!is_null($app_access_token) && !is_null($this->accessToken)) {
      $request_url = self::SMI_FB_TOKEN_DEBUG_BASE_URL . http_build_query($query_params, '', '&');
      $response = drupal_http_request($request_url);
      if (strlen($response->data) > 0) {
        $response = json_decode($response->data);
        if (!is_null($response)) {
          if ($response->data->is_valid) {
            $extended_access_token = $this->extendAccessToken();
            if (!is_null($extended_access_token)) {
              // Set the new access token.
              socialmedia_importer_application_save(array('id' => $this->id, 'access_token' => $extended_access_token));
              watchdog('socialmedia_importer', 'Access Token extended for %app', array('%app' => $this->appName));
              $is_valid = $response->data->is_valid;
            }
            else {
              watchdog('socialmedia_importer', 'Access Token for %app can not be extended.', array('%app' => $this->appName));
            }
          }
        }
      }
    }
    return $is_valid;
  }

  /**
   * Get an Application Access Token. This is used to debug an Access Token.
   */
  public function getApplicationAccessToken() {
    $access_token = NULL;
    $response_params = array();
    $query_params = array(
      'client_id' => $this->appId,
      'client_secret' => $this->appSecret,
      'grant_type' => 'client_credentials',
    );
    $request_url = self::SMI_FB_OAUTH_BASE_URL . http_build_query($query_params, '', '&');
    $response = drupal_http_request($request_url);
    if (is_object($response) && property_exists($response, 'data')) {
      parse_str($response->data, $response_params);
      if (array_key_exists('access_token', $response_params)) {
        $access_token = $response_params['access_token'];
      }
    }
    return $access_token;
  }

  /**
   * Get an Application Data.
   */
  public function getApplicationData() {
    $graph_url = "https://graph.facebook.com/me/feed?access_token=" . $this->accessToken;
    $response = drupal_http_request($graph_url);
    if (!is_null($response)) {
      if (property_exists($response, 'error')) {
        if ($response->error->type == "OAuthException") {
          // Retrieving a valid access token.
        }
        else {
          echo "other error has happened";
        }
      }
      else {
        $data_json = json_decode($response->data);
        echo json_encode($data_json->data);
        return $response;
      }
    }
  }

  /**
   * Validate a Facebook Application by App_id.
   *
   * @return bool
   *   Return TRUE if the Application is a valid Facebook Application.
   */
  public function validateApplication() {
    $is_valid = TRUE;
    $app_id = $this->appId;
    $graf_url = 'https://graph.facebook.com/' . $app_id;
    $response = drupal_http_request($graf_url);
    if (is_object($response)) {
      $data = json_decode($response->data);
      if (property_exists($data, 'error')) {
        $is_valid = FALSE;
      }
    }
    return $is_valid;
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
    $graph_url = "https://graph.facebook.com/me/permissions/?access_token=" . $this->accessToken;
    $options = array(
      'method' => 'DELETE',
    );
    $response = drupal_http_request($graph_url, $options);
    $is_deauthorize = array('success' => FALSE);
    if (!is_null($response)) {
      $response_data = json_decode($response->data);
      if (property_exists($response, 'error')) {
        if (is_object($response_data)) {
          if (property_exists($response_data, 'error')) {
            if ($response_data->error->type == "OAuthException") {
              $is_deauthorize['error_message'] = $response_data->error->message;
            }
            else {
              $is_deauthorize['error_message'] = t('Other error has happened');
            }
          }
        }
      }
      else {
        $is_deauthorize['success'] = TRUE;
      }
    }
    return $is_deauthorize;
  }
}
