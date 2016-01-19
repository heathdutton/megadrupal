<?php

/**
 * @file
 * Implementation of the MYDIGIPASS.COM OAuth2 callback function.
 */

/**
 * Page callback for 'mydigipass/callback'.
 */
function mydigipass_callback() {
  // Define the error message which will be returned as the page body in case
  // an error occurs while the function is being executed.
  $return_or_error = t('An error occurred while contacting MYDIGIPASS.COM. Please try again. If the problem persists, contact your site administrator.');

  // Check if the integration is enabled. When integration is not enabled, a
  // user could arrive at the callback page when using the single sign-on
  // functionality on the MYDIGIPASS.COM dashboard or when forcefully browsing
  // to 'mydigipass/callback'.
  if (variable_get('mydigipass_integration_enabled', 0) == 0) {
    return t('The integration with MYDIGIPASS.COM has been disabled on this website. It is not possible to logon using MYDIGIPASS.COM.');
  }

  // Check whether the client_secret and client_id have been set.
  $client_secret = variable_get('mydigipass_client_secret', '');
  $client_id = variable_get('mydigipass_client_id', '');
  if (empty($client_secret) || empty($client_id)) {
    drupal_set_message(
      t('The MYDIGIPASS.COM module has not been correctly configured on this Drupal installation. Either the client_id or the client_secret has not been properly configured.'),
      'error');
    watchdog(
      'mydigipass',
      'The MYDIGIPASS.COM module has not been correctly configured on this Drupal installation. Either the client_id or the client_secret has not been properly configured.',
      array(),
      WATCHDOG_ERROR);
    return $return_or_error;
  }

  // Check if the callback contained the error parameter. If so, something must
  // have gone wrong.
  if (isset($_GET['error'])) {
    $error = $_GET['error'];
    $error_description = (isset($_GET['error_description']) ? $_GET['error_description'] : "");

    drupal_set_message(
      t('An error occurred: Error: @error - Error description: @error_description',
        array('@error' => $error, '@error_description' => $error_description)),
      'error');
    watchdog(
      'mydigipass',
      'An error occurred: Error: @error - Error description: @error_description',
      array('@error' => $error, '@error_description' => $error_description),
      WATCHDOG_ERROR);
    return $return_or_error;
  }

  // Extract the authorisation code.
  $code = $_GET['code'];
  // Validate using regular expression that the authorisation code exists out
  // of [0-9a-z] characters.
  if (preg_match('/^[0-9a-z]+$/', $code) != 1) {
    drupal_set_message(
      t('An error occurred: the authorisation code does not have the expected format'),
      'error');
    watchdog(
      'mydigipass',
      'An error occurred: the authorisation code does not have the expected format',
      array(),
      WATCHDOG_ERROR);
    return $return_or_error;
  }

  // Extract the value of the state parameter.
  $state_string = (isset($_GET['state']) ? $_GET['state'] : "");
  // Validate using regular expression that, if the state parameter is set,
  // the state exists out of Base64 characters.
  if (!empty($state_string)) {
    if (preg_match('/^[a-zA-Z0-9+\/]+={0,2}$/', $state_string) != 1) {
      drupal_set_message(
        t('An error occurred: the state parameter does not have the expected format'), 'error');
      watchdog('mydigipass', 'An error occurred: the state parameter does not have the expected format', array(), WATCHDOG_ERROR);
      return $return_or_error;
    }
    $state_array = _mydigipass_decode_state_array($state_string);

    // Check whether the CSRF token is correct in the state array.
    if (!_mydigipass_check_csrf_token($state_array)) {
      drupal_set_message(t('An error occurred: the CSRF token in the state parameter has an incorrect value'), 'error');
      watchdog('mydigipass', 'An error occurred: the CSRF token in the state parameter has an incorrect value', array(), WATCHDOG_ERROR);
      return $return_or_error;
    }
  }
  else {
    $state_array = array();
  }

  // The following function call will perform the actual communication with
  // MYDIGIPASS.COM and will exchange the authorisation code for an access
  // token and additionally exchange the access token for user data.
  $user_data_array = _mydigipass_consume_authorisation_code($code);

  // Check the value of $user_data_array: if it is FALSE, then an error
  // occurred during the communication with MYDIGIPASS.COM.
  if (!$user_data_array) {
    // An error occurred during the process. Not necessary to report any errors
    // since this has already been done earlier. Just return.
    return $return_or_error;
  }

  // Log that the user connected via MDP.
  watchdog(
    'mydigipass',
    'Connection from MYDIGIPASS.COM user with ' . (isset($user_data_array['email']) ? 'email %email and ' : '') . 'UUID %uuid.',
    array(
      '%email' => (isset($user_data_array['email']) ? $user_data_array['email'] : ''),
      '%uuid' => $user_data_array['uuid'],
    ));
  // Mark this session as one in which a MYDIGIPASS.COM user is authenticated.
  $_SESSION['mydigipass_uuid'] = $user_data_array['uuid'];

  // Update the user data in the database.
  db_delete('mydigipass_user_data')
    ->condition('mdp_uuid', $user_data_array['uuid'])
    ->execute();
  // Insert the user data using a multi-insert query.
  $query = db_insert('mydigipass_user_data')
    ->fields(array('mdp_uuid', 'attribute_key', 'attribute_value'));
  foreach ($user_data_array as $key => $value) {
    $query->values(array(
      'mdp_uuid' => $user_data_array['uuid'],
      'attribute_key' => $key,
      'attribute_value' => $value,
    ));
  }
  $query->execute();

  // At this point, the end-user has authenticated himself to MYDIGIPASS.COM.
  // Check whether this end-user is already linked to a Drupal user.
  $query = db_select('mydigipass_user_link', 'mul')
    ->condition('mul.mdp_uuid', $user_data_array['uuid'])
    ->fields('mul', array('drupal_uid'));
  $result = $query->countQuery()->execute();

  if ($result->fetchField() == 1) {
    // The MYDIGIPASS.COM end-user is already linked to an existing Drupal
    // user, let's authenticate the Drupal user.
    $uid = $query->execute()->fetchField();
    $account = user_load($uid);

    // The following authentication flow is copied from the core openid module.
    if (!variable_get('user_email_verification', TRUE) || $account->login) {
      // Check if user is blocked.
      $state['values']['name'] = $account->name;
      user_login_name_validate(array(), $state);
      if (!form_get_errors()) {
        // Load global $user and perform final login tasks.
        $form_state['uid'] = $account->uid;
        user_login_submit(array(), $form_state);
      }
    }
    else {
      drupal_set_message(t('You must validate your email address for this account before logging in via MYDIGIPASS.COM.'), 'warning');
    }

    // Redirect the user to the front page.
    drupal_goto();

    return;
  }
  else {
    // The MYDIGIPASS.COM end-user is not yet linked to an existing Drupal
    // user.
    // Check if the user is logged in and if the state parameter is set.
    // If so, then the user clicked the 'Link to MYDIGIPASS.COM' button in his
    // profile.
    // Note that the value of the mydigipass_link_csrf_token does not have to
    // be checked anymore. It has already been done at the beginning of the
    // callback processing.
    if (user_is_logged_in() && !empty($_SESSION['mydigipass_link_csrf_token']) &&
        ($state_array['action'] == 'link')) {
      // Cleanup the session data: the link code is already consumed.
      unset($_SESSION['mydigipass_link_csrf_token']);

      global $user;
      // Link to currently logged on user.
      $fields = array(
        'drupal_uid' => $user->uid,
        'mdp_uuid' => $user_data_array['uuid'],
      );
      try {
        db_insert('mydigipass_user_link')->fields($fields)->execute();
      }
      catch (Exception $e) {
        watchdog_exception('mydigipass', $e);
        drupal_set_message(t('An error occurred while linking the user to MYDIGIPASS.COM'), 'error');
        return;
      }

      drupal_set_message(t('The user has been successfully linked to MYDIGIPASS.COM'));

      // Inform MYDIGIPASS.COM of the link. No need to check the result of
      // the call. If it failed, the function already sets the inconsistency
      // variable.
      _mydigipass_api_uuid_connected($user_data_array['uuid']);

      // Redirect to the user edit form (which contained the MYDIGIPASS.COM
      // connect button).
      drupal_goto('user/' . $user->uid . '/edit');
      return;
    }
    else {
      // Show the link user wizard or the mydigipass_user_register form.
      if (isset($state_array['action']) && ($state_array['action'] == 'register')) {
        drupal_goto('mydigipass/link/new_user');
      }
      else {
        drupal_goto('mydigipass/link');
      }
      return;
    }
  }

  return;
}

/**
 * Private helper function which consumes the OAuth authorisation code.
 *
 * This function exchanges the authorisation code for an access token. Using
 * the access token, it collects the end-user data from MYDIGIPASS.COM.
 *
 * @param string $code
 *   The authorisation code which was extracted from the callback URL.
 *
 * @return array|bool
 *   An array containing the end-user data or FALSE in case an error occurred.
 */
function _mydigipass_consume_authorisation_code($code) {
  // Step 1: exchange the authorisation code for an access token.
  $access_token = _mydigipass_callback_get_access_token($code);

  // Check if the function returned FALSE.
  if ($access_token === FALSE) {
    // A communication error occurred. An error message has already been
    // displayed and logged to watchdog.
    return FALSE;
  }

  // Validate using regular expression that the access token exists out of
  // [0-9a-z] characters.
  if (preg_match('/^[0-9a-z]+$/', $access_token) != 1) {
    drupal_set_message(
      t('An error occurred: the access token does not have the expected format'),
      'error');
    watchdog(
      'mydigipass',
      'An error occurred: the access token does not have the expected format. Returned token was %token',
      array('%token' => $access_token),
      WATCHDOG_ERROR);
    return FALSE;
  }

  // Step 2: exchange the access token for user data.
  $user_data = _mydigipass_callback_get_user_data($access_token);

  // Check if the function returned FALSE.
  if ($user_data === FALSE) {
    // A communication error occurred. An error message has already been
    // displayed and logged to watchdog.
    return FALSE;
  }

  // Check if the UUID is present.
  if (is_array($user_data) && isset($user_data['uuid']) &&
    !empty($user_data['uuid'])) {
    return $user_data;
  }
  else {
    return FALSE;
  }
}

/**
 * Helper function which exchanges the authorisation code for an access token.
 *
 * @param string $code
 *   The authorisation code which was provided in the callback URL.
 *
 * @return string|bool
 *   If no errors occurred: a string containing the access token.
 *   If errors occurred: FALSE
 */
function _mydigipass_callback_get_access_token($code) {
  // Get the URL of the token endpoint.
  $token_endpoint = _mydigipass_get_endpoint_url('token_endpoint');

  // If either the authorisation code or the token endpoint URL is empty,
  // return FALSE.
  if (empty($code) || empty($token_endpoint)) {
    return FALSE;
  }

  // Exchange the authorisation code for an access token.
  $post_data = array(
    'code' => $code,
    'client_secret' => variable_get('mydigipass_client_secret', ''),
    'client_id' => variable_get('mydigipass_client_id', ''),
    'redirect_uri' => variable_get('mydigipass_callback_url', url('mydigipass/callback', array('absolute' => TRUE))),
    'grant_type' => 'authorization_code',
  );

  // The HTTP request options.
  $options = array(
    'method' => 'POST',
    'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
    'data' => http_build_query($post_data, '', '&'),
  );

  $ssl_context_array = _mydigipass_create_ssl_context();
  if ($ssl_context_array === FALSE) {
    // A validation check failed.
    return FALSE;
  }
  $options['context'] = stream_context_create($ssl_context_array);
  $result = drupal_http_request($token_endpoint, $options);

  // Fail secure: set return value to FALSE.
  $return = FALSE;

  switch ($result->code) {
    case 200:
    case 301:
    case 302:
      $access_token_array = json_decode($result->data, TRUE);
      $return = $access_token_array['access_token'];
      break;

    default:
      watchdog('mydigipass', 'An error occurred while contacting MYDIGIPASS.COM: "%error".', array('%error' => $result->code . ' ' . $result->error), WATCHDOG_WARNING);
      drupal_set_message(t('An error occurred while contacting MYDIGIPASS.COM.'));
  }
  return $return;
}

/**
 * Private helper function which exchanges the access token for the user data.
 *
 * @param string $access_token
 *   The access token which was received from MYDIGIPASS.COM.
 *
 * @return array|bool
 *   If no errors occurred: An associative array which contains the user data
 *                         in 'attribute_name' => 'attribute_value' pairs.
 *   If errors occurred: FALSE
 */
function _mydigipass_callback_get_user_data($access_token) {
  // Get the URL of the user data endpoint.
  $user_data_endpoint = _mydigipass_get_endpoint_url('data_endpoint');

  // If either the access token  or the user data endpoint URL is empty, return
  // an empty array.
  if (empty($access_token) || empty($user_data_endpoint)) {
    return array();
  }

  // The HTTP request options.
  $options = array(
    'method' => 'GET',
    'headers' => array('Authorization' => 'Bearer ' . $access_token),
  );

  // Call MYDIGIPASS.COM User Data Endpoint.
  $ssl_context_array = _mydigipass_create_ssl_context();
  if ($ssl_context_array === FALSE) {
    // A validation check failed.
    return FALSE;
  }
  $options['context'] = stream_context_create($ssl_context_array);
  $result = drupal_http_request($user_data_endpoint, $options);

  // Fail secure: set return value to FALSE.
  $return = FALSE;

  switch ($result->code) {
    case 200:
    case 301:
    case 302:
      $return = json_decode($result->data, TRUE);
      break;

    default:
      watchdog('mydigipass', 'An error occurred while contacting MYDIGIPASS.COM: "%error".', array('%error' => $result->code . ' ' . $result->error), WATCHDOG_WARNING);
      drupal_set_message(t('An error occurred while contacting MYDIGIPASS.COM.'));
  }
  return $return;
}

/**
 * Private helper function which checks the CSRF token in the state parameter.
 *
 * In every OAuth call to MYDIGIPASS.COM a token is added which allows to
 * check whether the call to MYDIGIPASS.COM originated from within this session
 * (i.e. whether the end-user clicked on the button).
 *
 * @param array $state_array
 *   An array containing the decoded value of the state array.
 *
 * @return bool
 *   Returns TRUE if the CSRF token was correct, returns FALSE in all other
 *   cases.
 */
function _mydigipass_check_csrf_token(array $state_array) {
  switch ($state_array['action']) {
    case 'login':
      $return = (isset($_SESSION['mydigipass_login_csrf_token']) && ($state_array['csrf_token'] == $_SESSION['mydigipass_login_csrf_token']));
      break;

    case 'register':
      $return = (isset($_SESSION['mydigipass_register_csrf_token']) && ($state_array['csrf_token'] == $_SESSION['mydigipass_register_csrf_token']));
      break;

    case 'link':
      $return = (isset($_SESSION['mydigipass_link_csrf_token']) && ($state_array['csrf_token'] == $_SESSION['mydigipass_link_csrf_token']));
      break;

    default:
      $return = FALSE;
  }

  return $return;
}
