<?php

/**
 * Menu callback for session redirection page.
 */
function services_sso_client_try_provided_session_pair_page($key, $secret) {
  $data = services_sso_client_session_try_provided_pair($key, $secret);
  if (!services_sso_client_session_login($data)) {
    drupal_access_denied();
  }
  drupal_goto('user');
}

/**
 * This will handle logging in of a SSO server session locally.
 *
 * @param string $key session key
 * @param string $secret session secret
 *
 * @return object $data
 *  If data response is existent and valid, return the data object.
 *  Otherwise return FALSE.
 */
function services_sso_client_session_try_provided_pair($key, $secret) {
  // See if we can really connect with this session
  // Construct proper endpoint URL for the services resource
  $endpoint_url = variable_get('services_sso_client_server_address', '') . '/' . variable_get('services_sso_client_server_endpoint', '');
  // Construct proper cookie using the cookie session information.
  $headers = array(
    'Content-Type' => 'application/json',
    'Cookie' => $key . '=' . $secret,
    'X-CSRF-Token' => _services_sso_client_get_csrf_token($key, $secret),
  );

  // Make request to REST services system.connect
  $response = drupal_http_request($endpoint_url . '/system/connect.json', array('headers' => $headers, 'method' => 'POST'));

  // Decode response and update the local Drupal user in the DB if necessary.
  if (services_sso_client_verify_response($response)) {
    $data = json_decode($response->data);

    // The session name isn't set sometimes, manually set it if this is the case
    if (empty($data->session_name)) {
      $data->session_name = $key;
    }

    return $data;
  }

  return FALSE;
}

/**
 * Login a remote user locally based on valid session info and user object
 * in data return from services 3 connect.json.
 *
 * @param object $data
 *  A successful data return from connect.json containing session name and remote
 *  user object.
 *
 * @return object $account
 *  The end result is that the remote user is successfully cloned locally, if
 *  necessary. The user is then logged in locally. A local user object is returned.
 */
function services_sso_client_session_login($data) {
  if (!empty($data) && !empty($data->user)) {
    // Copy the user object to $_SESSION.
    $_SESSION['services_sso_client']['data'] = $data;

    $remote_user = $data->user;

    if (!empty($remote_user->uid)) {
      // Make sure this guy isn't one of the ignored accounts
      $ignored_names = _services_sso_client_ignore_accounts();
      if (in_array($remote_user->name, $ignored_names)) {
        return;
      }

      // Construct proper endpoint URL for the services resource
      $endpoint_url = variable_get('services_sso_client_server_address', '') . '/' . variable_get('services_sso_client_server_endpoint', '');
      // Construct proper cookie using the cookie session information.
      $headers = array('Cookie' => $data->session_name . '=' . $data->sessid);

      // Make request to REST services user.retrieve
      $response = drupal_http_request($endpoint_url . '/user/' . $remote_user->uid, array('headers' => $headers));
      $data = json_decode($response->data);

      if (!empty($data->uid)) {
        $remote_user = $data;

        $_SESSION['services_sso_client']['data']->user = $remote_user;

        // Set the common domain cookie
        services_sso_client_session_set_common_cookie($data);

        // Update local Drupal user in DB and log this guy in.
        return services_sso_client_update_remote_process($remote_user, FALSE);
      }
      else {
        $remote_user = NULL;
      }
    }
  }

  return FALSE;
}

/**
 * Set the common cookie provided valid services data return.
 */
function services_sso_client_session_set_common_cookie($data, $time = 2592000) {
  global $base_path;
  $common_cookie_domain = _services_sso_client_get_common_cookie_domain();

  setcookie($data->session_name, $data->sessid, REQUEST_TIME + $time, $base_path, $common_cookie_domain, FALSE, TRUE);
}
