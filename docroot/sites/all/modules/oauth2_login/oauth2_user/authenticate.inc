<?php
/**
 * @file
 * Misc utility functions.
 */

/**
 * Implements hook_init().
 */
function oauth2_user_init() {
  // Check whether this is a redirect after login,
  // and if yes call the submit function again.
  if (oauth2_user_is_authenticated() and isset($_SESSION['oauth2_user']['form_state'])) {
    oauth2_user_form_resubmit();
  }
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

  // Save form_state
  $_SESSION['oauth2_user']['form_state'] = $form_state;

  // Redirect to login.
  oauth2_login();
}

/**
 * If there is a $_SESSION['oauth2_user']['form_state'],
 * then this is a redirect after login. Call the submit function again.
 */
function oauth2_user_form_resubmit() {
  $form_state = $_SESSION['oauth2_user']['form_state'];
  unset($_SESSION['oauth2_user']['form_state']);
  $form_id = $form_state['values']['form_id'];
  $submit = $form_id . '_submit';
  if (function_exists($submit)) {
    $submit(NULL, $form_state);
  }
}
