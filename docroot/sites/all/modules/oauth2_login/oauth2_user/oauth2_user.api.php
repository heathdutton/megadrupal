<?php
/**
 * @file
 * Functions that can be used by other modules.
 */

/**
 * Get the profile of the oauth2 user.
 */
function oauth2_user_get() {
  // If it is already cached, return the cached one.
  if (isset($_SESSION['oauth2_user']['profile'])) {
    return $_SESSION['oauth2_user']['profile'];
  }
  else {
    return oauth2_user_get_from_server();
  }
}

/**
 * Save the profile of the user on session.
 */
function oauth2_user_save($oauth2_user) {
  // Allow other modules to customize the oauth2 user profile.
  drupal_alter('oauth2_user', $oauth2_user);
  $_SESSION['oauth2_user']['profile'] = $oauth2_user;
}

/**
 * Remove oauth2_user from the session.
 * This will cause the profile to be refreshed from the server.
 */
function oauth2_user_expire() {
  unset($_SESSION['oauth2_user']['profile']);
}

/**
 * Implements hook_profile_fields_alter().
 */
function MODULE_profile_fields_alter(&$profile_fileds) {
  // Add these additional fields to the user profile.
  // These extra fields can be defined by the server
  // on hook_oauth2_loginprovider_userprofile_alter().
  $profile_fields += [
    'projects',
    'permissions',
  ];
}

/**
 * Implements hook_oauth2_user_alter().
 *
 * $oauth2_user is the remote user profile that comes from the server.
 */
function MODULE_oauth2_user_alter(&$oauth2_user) {
  // Keep only the fields of the user profile that we are interested in.
  $remote_user = [
    'projects' => $oauth2_user['projects'],
    'permissions' => $oauth2_user['permissions'],
    'uid' => $oauth2_user['identifier'],
    'name' => $oauth2_user['displayName'],
  ];
  $oauth2_user = $remote_user;
}

/**
 * Return true if the user has a valid oauth2 access token.
 */
function oauth2_user_is_authenticated() {
  $server_url = variable_get('oauth2_login_oauth2_server', '');
  $token_endpoint = $server_url . '/oauth2/token';
  $client_id = variable_get('oauth2_login_client_id', '');
  $auth_flow = 'server-side';

  // Get the current access_token.
  $id = md5($token_endpoint . $client_id . $auth_flow);
  $token = oauth2_client_get_token($id);

  // Check the access token.
  if (empty($token['access_token'])) return FALSE;
  if ($token['expiration_time'] < (time() + 10))  return FALSE;

  return TRUE;
}

/**
 * Authenticate the user (redirect to login).
 * But first save the given form_state in session.
 */
function oauth2_user_authenticate($form_state, $redirection = FALSE) {
  if ($redirection) {
    // We are in a redirect-after-login, but login has failed or was cancelled.
    // In this case we clear the session variable so that it does not keep
    // redirecting.
    unset($_SESSION['oauth2_user']['form_state']);
    return;
  }

  // Save form_state.
  $_SESSION['oauth2_user']['form_state'] = $form_state;

  // Redirect to login.
  oauth2_login();
}

/**
 * Set curl options for development, testing and debug.
 *
 * It can be used when defining a wsclient service, like this:
 *   $service->settings += oauth2_user_wsclient_dev_settings();
 */
function oauth2_user_wsclient_dev_settings() {
  $skipssl = variable_get('oauth2_login_skipssl', TRUE);
  $proxy = variable_get('oauth2_login_proxy', '');

  $dev_settings = array();
  if ($skipssl) {
    // Skip checking the SSL certificate, for testing.
    $dev_settings['curl options'] = [
      CURLOPT_SSL_VERIFYPEER => FALSE,
      CURLOPT_SSL_VERIFYHOST => FALSE,
    ];
  }
  if ($proxy) {
    $dev_settings['curl options'][CURLOPT_PROXY] = $proxy;
  }
  return $dev_settings;
}

/**
 * Return authentication settings.
 *
 * It can be used when defining a wsclient service, like this:
 *   $service->settings += oauth2_user_wsclient_auth_settings();
 */
function oauth2_user_wsclient_auth_settings() {
  $server_url = variable_get('oauth2_login_oauth2_server', '');
  $client_id = variable_get('oauth2_login_client_id', '');
  $client_secret = variable_get('oauth2_login_client_secret', '');

  $token_endpoint = $server_url . '/oauth2/token';
  $authorization_endpoint = $server_url . '/oauth2/authorize';
  $redirect_uri = url('oauth2/authorized', ['absolute' => TRUE]);

  $auth_settings['authentication']['oauth2'] = [
    'token_endpoint' => $token_endpoint,
    'auth_flow' => 'server-side',
    'client_id' => $client_id,
    'client_secret' => $client_secret,
    'redirect_uri' => $redirect_uri,
    'authorization_endpoint' => $authorization_endpoint,
    'scope' => 'user_profile',
  ];

  return $auth_settings;
}
