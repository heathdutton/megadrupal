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
 * Return true if the user has a valid oauth2 access_token.
 */
function oauth2_user_is_authenticated() {
  $access_token = oauth2_login_get_access_token($redirect=FALSE);
  return ($access_token ? TRUE : FALSE);
}

/**
 * Authenticate the user (redirect to login).
 * But first save the given form_state in session.
 */
function oauth2_user_authenticate($form_state, $redirection = FALSE) {
  if (user_is_logged_in()) {
    global $user;
    $user = drupal_anonymous_user();
    $dest = drupal_get_destination();
    oauth2_login(NULL, $dest['destination']);
    return;
  }

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
