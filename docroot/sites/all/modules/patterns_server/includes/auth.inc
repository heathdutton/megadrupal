<?php
/**
 * @file
 * Code related to authorizing clients on performing operations on the server
 * The code is based on drupal authentication functions. The changes
 * are due to the fact that we don't want to send the password in plain text,
 * so we must adapt the authentication to deal with an already encrypted string.
 */

/**
 * Authenticates the user.
 * A validate handler on the login form. Check supplied username/password
 * against local users table. If successful, $form_state['uid']
 * is set to the matching user ID.
 */
function _patterns_server_login_authenticate($user, $password) {
  $status = PATTERNS_SUCCESS;
  if (empty($user)) {
    return patterns_results(PATTERNS_ERR, t('Username cannot be empty'));
  }
  if (empty($password)) {
    return patterns_results(PATTERNS_ERR, t('Password cannot be empty'));
  }
  $password = trim($password);
  /* Do not allow any login from the current user's IP if the limit has been
   * reached. Default is 50 failed attempts allowed in one hour. This is
   * independent of the per-user limit to catch attempts from one IP to log
   * in to many different user accounts.  We have a reasonably high limit
   * since there may be only one apparent IP for all users at an institution.
   */
  if (!flood_is_allowed('failed_login_attempt_ip', variable_get('user_failed_login_ip_limit', 50), variable_get('user_failed_login_ip_window', 3600))) {
    return patterns_results(PATTERNS_ERR, t('Too many failed logins attempts from your ip address. You are temporarily blocked. Please try again later.'));
  }
  $account = db_query("SELECT * FROM {users} WHERE name = :name", array(':name' => $user))->fetchObject();
  if (!$account) {
    return patterns_results(PATTERNS_ERR, t('No such user found: :name', array(':name' => $user)));
  }
  if ($account->status != 1) {
    return patterns_results(PATTERNS_ERR, t('Account not active: :name', array(':name' => $user)));
  }
  if (variable_get('user_failed_login_identifier_uid_only', FALSE)) {
    // Register flood events based on the uid only, so they apply for any
    // IP address. This is the most secure option.
    $identifier = $account->uid;
  }
  else {
    // The default identifier is a combination of uid and IP address. This
    // is less secure but more resistant to denial-of-service attacks that
    // could lock out all users with public user names.
    $identifier = $account->uid . '-' . ip_address();
  }
  // Don't allow login if the limit for this user has been reached.
  // Default is to allow 5 failed attempts every 6 hours.
  if (!flood_is_allowed('failed_login_attempt_user', variable_get('user_failed_login_user_limit', 5), variable_get('user_failed_login_user_window', 21600), $identifier)) {
    return patterns_results(PATTERNS_ERR, t('You have reached the maximum number of login attempts for your username: :name. Your account is temporarily disabled, please try later.', array(':name' => $user)));;
  }
  if (_patterns_server_check_password($user, $password)) {
    _patterns_server_login_success($identifier);
    return patterns_results(PATTERNS_SUCCESS, t('Login successful'));
  }
  else {
    _patterns_server_login_fail($identifier);
    return patterns_results(PATTERNS_ERR, t('Login failed'));
  }
}

/**
 * Checks the password.
 */
function _patterns_server_check_password($user, $password) {
  $account = user_load_by_name($user);
  return $account && $account->pass == $password;
}

/**
 * Checks the token.
 */
function _patterns_server_token_authenticate($user, $token) {
  $queryparam = array(':uid' => $user, ':token' => $token);
  $found = db_query("SELECT * FROM {patterns_server_tokens} WHERE uid = :uid AND token = :token", $queryparam)->fetchObject();
  return $found;
}

/**
 * Login success event.
 */
function _patterns_server_login_success($identifier) {
  if (!empty($identifier)) {
    flood_clear_event('failed_login_attempt_user', $identifier);
  }
}

/**
 * Login failed event.
 */
function _patterns_server_login_fail($identifier) {
  flood_register_event('failed_login_attempt_ip', variable_get('user_failed_login_ip_window', 3600));
  if (!empty($identifier)) {
    flood_register_event('failed_login_attempt_user', variable_get('user_failed_login_user_window', 21600), $identifier);
  }
}

/**
 * Adds an entry in the watchdog in case there was a login error.
 */
function patterns_server_log_failed_login($user, $pwd) {
  watchdog('patterns_server', 'username :user failed authentication with password :pwd', array(':user' => $user, ':pwd' => $pwd), WATCHDOG_NOTICE);
}

