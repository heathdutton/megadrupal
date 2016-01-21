<?php

/**
 * @file
 * Handle the authentication process of xing.
 */

/**
 * Process the OAuth2 logic to integrate with xing.
 *
 * Request which will cause a client redirect to xing for auth code.
 * On return from that we validate the code and get back a
 * secure token which gives us access to the users Xing
 * account. Then we going and associate the users account
 * and log them in.
 *
 * @see https://dev.xing.com/docs/authentication
 */
function xing_connect_process_oauth2() {
  $parameters = drupal_get_query_parameters();
  if (!empty($parameters)
    && !empty($parameters['oauth_verifier'])
    && !empty($parameters['oauth_token'])
  ) {
    // Check the request made on same time.
    if ($_SESSION['state'] == $parameters['state']) {
      xing_connect_access_token($parameters['oauth_token'], $parameters['oauth_verifier'], $_SESSION['oauth_token_secret']);
    }
    else {
      // CSRF attack?
      drupal_goto('<front>');
    }
  }
  else {
    // Start authorization process.
    xing_connect_request_token();
  }
}

/**
 * Redirect a user to xing to obtain a request Code.
 */
function xing_connect_request_token() {
  // Get a one time state used to help prevent CSRF attack.
  $_SESSION['state'] = md5(REQUEST_TIME);
  $params = array(
    'oauth_signature_method' => 'PLAINTEXT',
    'oauth_version' => '1.0',
    'oauth_signature' => variable_get('xing_connect_skey') . '&',
    'oauth_timestamp' => REQUEST_TIME,
    'oauth_nonce' => md5(uniqid(mt_rand(), TRUE)),
    'oauth_consumer_key' => variable_get('xing_connect_ckey'),
    'oauth_callback' => url(
      'xing/auth/',
      array(
        'query' => array('state' => $_SESSION['state']),
        'absolute' => TRUE,
        'https' => variable_get('https', FALSE),
      )
    ),
  );
  // Redirect the user to Xing to get an request code.
  $xing_request_url = variable_get(
    'xing_request_token_url',
    'https://api.xing.com/v1/request_token'
  );
  $url = url($xing_request_url, array('query' => $params, 'external' => TRUE));
  // Lets make the request to Xing to validate the request code.
  $xing_response = drupal_http_request($url);
  try {
    if ($xing_response->code == '201') {
      parse_str($xing_response->data, $xing_request_token_response);
      return xing_connect_authorize($xing_request_token_response);
    }
    else {
      // Lets try and get as much info as we can for the error logging.
      $xing_request_token_response = drupal_json_decode($xing_response->data);
      $xing_request_token_error = empty($xing_request_token_response['error_name']) ? '' : $xing_request_token_response['error_name'];
      $xing_request_token_error_description = empty($xing_request_token_response['message']) ? '' : $xing_request_token_response['message'];
      $mesg_array = array(
        '@code' => $xing_request_token_response->code,
        '@message' => $xing_request_token_response->error,
        '@xing_error' => $xing_request_token_error,
        '@xing_error_description' => $xing_request_token_error_description,
      );
      watchdog('xing_connect', 'Unable to access request token for xing: code: @code, messsage: @message, xing error: @xing_error, xing error description: @xing_error_description', $mesg_array, WATCHDOG_ERROR);
      drupal_set_message(t('An error occurred and at this time we are unable to connect to Xing.'), 'error');
      drupal_goto('<front>');
    }
  }
  catch (Exception $e) {
    watchdog('xing_connect', 'Unable to connect to Xing.', array(), WATCHDOG_ERROR);
    return FALSE;
  }
}

/**
 * Redirect to xing to obtain a Auth Code based on request code.
 */
function xing_connect_authorize($request_token) {
  $_SESSION['oauth_token_secret'] = $request_token['oauth_token_secret'];
  $params = array(
    'oauth_token' => $request_token['oauth_token'],
  );
  // Redirect the user to Xing to get an auth code.
  $xing_authorize_url = variable_get('xing_authorize', 'https://api.xing.com/v1/authorize');
  $url = url($xing_authorize_url, array('query' => $params, 'external' => TRUE));
  xing_connect_external_goto(
    $url,
    array('query' => $params, 'external' => TRUE, 'https' => TRUE)
  );
}

/**
 * Redirect a user to xing to obtain a access Code.
 */
function xing_connect_access_token($oauth_token, $oauth_verifier, $oauth_token_secret) {
  $params = array(
    'oauth_consumer_key' => variable_get('xing_connect_ckey'),
    'oauth_token' => $oauth_token,
    'oauth_signature_method' => 'PLAINTEXT',
    'oauth_version' => '1.0',
    'oauth_signature' => variable_get('xing_connect_skey') . '&' . $oauth_token_secret ,
    'oauth_timestamp' => REQUEST_TIME,
    'oauth_nonce' => md5(uniqid(mt_rand(), TRUE)),
    'oauth_verifier' => $oauth_verifier,
  );
  // Redirect the user to Xing to get an access code.
  $xing_request_url = variable_get('xing_access_token_url', 'https://api.xing.com/v1/access_token');
  $url = url($xing_request_url, array('query' => $params, 'external' => TRUE));
  // Lets make the request to Xing to validate the access code.
  $access_response = drupal_http_request($url);
  try {
    if ($access_response->code == '201') {
      parse_str($access_response->data, $xing_access_token_response);
      return xing_connect_authentication($xing_access_token_response);
    }
    else {
      // Lets try and get as much info as we can for the error logging.
      $xing_access_token_response = drupal_json_decode($access_response->data);
      $xing_access_token_error = empty($xing_access_token_response['error_name']) ? '' : $xing_access_token_response['error_name'];
      $xing_access_token_error_description = empty($xing_access_token_response['message']) ? '' : $xing_access_token_response['message'];
      $mesg_array = array(
        '@code' => $xing_access_token_response->code,
        '@message' => $xing_access_token_response->error,
        '@xing_error' => $xing_access_token_error,
        '@xing_error_description' => $xing_access_token_error_description,
      );
      watchdog('xing_connect', 'Unable to access token for xing: code: @code, messsage: @message, xing error: @xing_error, xing error description: @xing_error_description', $mesg_array, WATCHDOG_ERROR);
      drupal_set_message(t('An error occurred and at this time we are unable to connect to Xing.'), 'error');
      drupal_goto('<front>');
    }
  }
  catch (Exception $e) {
    watchdog('xing_connect', 'Unable to connect to Xing.', array(), WATCHDOG_ERROR);
    return FALSE;
  }
}

/**
 * Handle authenticating Xing account and getting user personal information.
 *
 * @param array $access_token
 *   Xing access token contain oauth_token, oauth_token_secret and user_id.
 *
 * @see https://dev.xing.com/docs/authentication
 * @see http://oauth.net/core/1.0a/
 */
function xing_connect_authentication(array $access_token) {
  if (empty($access_token)) {
    drupal_set_message(t('An error occurred and at this time we are unable to connect to Xing.'), 'error');
    drupal_goto('<front>');
  }

  $xing_candidate_info = array();
  // Get information from user access token.
  $xing_candidate_info['data'] = xing_connect_candidate_get_information($access_token);
  if (!empty($xing_candidate_info['data'])) {
    if (!empty($xing_candidate_info['data']['users'][0]['active_email'])) {
      $account = user_load_by_mail($xing_candidate_info['data']['users'][0]['active_email']);
      if (!empty($account)) {
        if ($account->status) {
          $form_state = array();
          $form_state['uid'] = $account->uid;
          user_login_submit(array(), $form_state);
          drupal_set_message(t('Welcome !!! You have been logged in with the username @username', array('@username' => $account->name)));
          drupal_goto(variable_get('xing_connect_post_login_url', '<front>'));
        }
        else {
          drupal_set_message(t('You could not be logged in as your account is blocked. Contact site administrator to activate your account.'), 'error');
          drupal_goto('<front>');
        }
      }
      else {
        if (variable_get('xing_connect_login')) {
          // Create the drupal user.
          // This will generate a random password, you could set your own here.
          $xing_username = (isset($xing_candidate_info['data']['users'][0]['display_name']) ? $xing_candidate_info['data']['users'][0]['display_name'] : $xing_candidate_info['data']['users'][0]['first_name']);
          $drupal_username_generated = xing_connect_unique_user_name($xing_username);
          $password = user_password(8);
          // Set up the user fields.
          $fields = array(
            'name' => $drupal_username_generated,
            'mail' => $xing_candidate_info['data']['users'][0]['active_email'],
            'pass' => $password,
            'status' => 1,
            'init' => 'email address',
            'roles' => array(
              DRUPAL_AUTHENTICATED_RID => 'authenticated user',
            ),
          );
          if (variable_get('user_pictures', 0)) {
            $picture_directory = file_default_scheme() . '://' . variable_get('user_picture_path', 'pictures/');
            file_prepare_directory($picture_directory, FILE_CREATE_DIRECTORY);
            $file = system_retrieve_file($xing_candidate_info['data']['users'][0]['photo_urls']['size_1024x1024'], $picture_directory . '/' . $xing_candidate_info['data']['users'][0]['display_name'] . '.jpg', TRUE, FILE_EXISTS_RENAME);
            if (is_object($file)) {
              $fields['picture'] = $file->fid;
            }
          }
          drupal_alter("xing_connect_register", $fields, $xing_candidate_info['data']);

          // The first parameter is left blank so a new user is created.
          $account = user_save('', $fields);
          // If you want to send the welcome email, use the following code.
          // Manually set the password so it appears in the e-mail.
          $account->password = $fields['pass'];
          // Send the e-mail through the user module.
          drupal_mail('user', 'register_no_approval_required', $account->mail, NULL, array('account' => $account), variable_get('site_mail', 'admin@drupalsite.com'));
          drupal_set_message(t('You have been registered with the username @username', array('@username' => $account->name)));
          drupal_goto("<front>");
        }
        else {
          drupal_set_message(t('There was no account with the email addresse @email found. Please register before trying to login.', array('@email' => $xing_candidate_info['data']['users'][0]['active_email'])), 'error');
          drupal_goto("<front>");
        }
      }
    }
    else {
      drupal_set_message(t("Though you have authorised the Xing app to access your profile, you don't have the permission to access email address. Please contact site administrator of xing."), "error");
      drupal_goto("<front>");
    }
  }
  else {
    drupal_set_message(t('An error occurred while fetching information from Xing.'), 'error');
    drupal_goto('<front>');
  }
}

/**
 * Here we get the user details from xing.
 *
 * @param string $access_token_parameters
 *   Xing access_token.
 */
function xing_connect_candidate_get_information($access_token_parameters) {
  // Fetching field from xing services.
  $xing_fields = variable_get(
    'xing_account_search_fields',
    'first_name,last_name,active_email,display_name,photo_urls.size_1024x1024'
  );

  $params = array(
    'fields' => $xing_fields,
    'oauth_consumer_key' => variable_get('xing_connect_ckey'),
    'oauth_token' => $access_token_parameters['oauth_token'],
    'oauth_signature_method' => 'PLAINTEXT',
    'oauth_version' => '1.0',
    'oauth_signature' => variable_get('xing_connect_skey') . '&' . $access_token_parameters['oauth_token_secret'],
    'oauth_timestamp' => REQUEST_TIME,
    'oauth_nonce' => md5(uniqid(mt_rand(), TRUE)),
    'format' => 'json',
  );

  // Xing search request url.
  $xing_url = variable_get('xing_account_search_path', 'https://api.xing.com/v1/users') . '/' . $access_token_parameters['user_id'];
  $url = url(
    $xing_url,
    array(
      'query' => $params,
      'external' => TRUE,
      'https' => variable_get('https', FALSE),
    )
  );

  $response = drupal_http_request($url);
  try {
    if ($response->code == '200') {
      return drupal_json_decode($response->data);
    }
    else {
      // Lets try and get as much info as we can for the error logging.
      $xing_response_data = drupal_json_decode($response->data);
      $xing_error = empty($xing_response_data['error_name']) ? '' : $xing_response_data['error_name'];
      $xing_error_description = empty($xing_response_data['message']) ? '' : $xing_response_data['message'];
      $mesg_array = array(
        '@code' => $response->code,
        '@message' => $response->error,
        '@xing_error' => $xing_error,
        '@xing_error_description' => $xing_error_description,
      );
      watchdog('xing_connect', 'Unable access account details while user data from Xing: code: @code, messsage: @message, xing error: @xing_error, xing error description: @xing_error_description', $mesg_array, WATCHDOG_ERROR);
      drupal_set_message(t('An error occurred and at this time we are unable to connect to Xing.'), 'error');
      drupal_goto('<front>');
    }
  }
  catch (Exception $e) {
    watchdog('xing_connect', 'Unable to connect to Xing.', array(), WATCHDOG_ERROR);
    return FALSE;
  }
}
