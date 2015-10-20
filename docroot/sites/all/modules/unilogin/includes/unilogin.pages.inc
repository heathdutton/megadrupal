<?php
/**
 * @file
 * Page callbacks for the UNI•Login module.
 */

/**
 * Redirect the user to UNI•Login if he's not already logged in.
 */
function unilogin_page_redirect() {
  if (user_is_logged_in()) {
    // Redirect the user back to the front page if destination is not
    // set in the URL.
    return drupal_goto('<front>');
  }

  $url = unilogin_get_url();

  return drupal_goto($url);
}

/**
 * Check the user's authentication.
 *
 * This is where the browser is sent to after logging in at UNI•Login's
 * site.
 */
function unilogin_page_authcheck() {
  // If we are not redirected from external UNI•Login service do nothing.
  if (!_unilogin_is_in_unilogin_callback_state()) {
    return t('No UNI•Login authentication data present in request.');
  }

  // If unilogin user authentication fails, and user related unilogin
  // data is present show an error.
  if (!_unilogin_verify_params($_GET['auth'], $_GET['timestamp'], $_GET['user'])) {
    drupal_set_message(t('UNI•Login authentication failed.'), 'error');
  }
  // Log the user in.
  else {
    _unilogin_authenticate($_GET['user'], $_GET['timestamp'], $_GET['auth']);
  }

  // Allow the login destination to be changed via a hook.
  $destination = (!empty($_GET['destination'])) ? $_GET['destination'] : '';
  drupal_alter('unilogin_login_destination', $destination);
  $_GET['destination'] = $destination;

  // Use the destination from the URL to redirect. If none is set, go
  // to the front page.
  drupal_goto('<front>');
}

/**
 * Logs in user if authenticated via unilogin.
 *
 * @param string $unilogin_id
 *   The unilogin id of a user.
 * @param integer $timestamp
 *   UNI•Login timestamp.
 * @param string $auth
 *   Auth token.
 *
 * @return mixed
 *   It either athenticates and redirects or returns FALSE.
 */
function _unilogin_authenticate($unilogin_id, $timestamp, $auth) {
  if (sizeof(module_implements('unilogin_authenticate')) > 0) {
    // Allow other modules to decide authentication.
    module_invoke_all('unilogin_authenticate', $unilogin_id, $timestamp, $auth);

    // Save ticket to db so it cannot be reused.
    _unilogin_db_save_ticket($auth, $timestamp, $unilogin_id);
  }
}

/**
 * Implements hook_unilogin_authenticate().
 */
function unilogin_unilogin_authenticate($unilogin_id, $timestamp, $auth) {
  // Register the user, if he does not exist, and then log him in.
  user_external_login_register(
    unilogin_authname($unilogin_id),
    'unilogin'
  );

  // Congratulate the user.
  drupal_set_message(t('You succesfully logged in via UNI•Login.'));
}

/**
 * Decide if we are in unilogin callback state.
 *
 * @return boolean
 *   UNI•Login callback state or not?
 */
function _unilogin_is_in_unilogin_callback_state() {
  // If there is a logged in user we don't want to do anything.
  if (user_is_logged_in()) {
    return FALSE;
  }
  // If neither of these $_GET params are present
  // we are not in unilogin callback state.
  if (
    empty($_GET['user'])
    || empty($_GET['auth'])
    || empty($_GET['timestamp'])
  ) {
    return FALSE;
  }
  // Auth has to contain 32 alpha numeric characters.
  if (!preg_match('#^[a-z0-9]{32}$#', $_GET['auth'])) {
    return FALSE;
  }
  // Timestamp has to contain 14 numeric characters long.
  if (!preg_match('#^[0-9]{14}$#', $_GET['timestamp'])) {
    return FALSE;
  }
  return TRUE;
}

/**
 * Verify auth token.
 *
 * @param string $auth
 *   Auth token.
 * @param integer $timestamp
 *   UNI•Login timestamp.
 * @param string $unilogin_id
 *   UNI•Login user id.
 *
 * @return boolean
 *   Is the returned auth token correct?
 */
function _unilogin_verify_auth($auth, $timestamp, $unilogin_id) {
  return $auth == md5(
    $timestamp
    . variable_get('unilogin_secret', '')
    . $unilogin_id
  );
}

/**
 * Check if ticket has been used.
 *
 * @param string $auth
 *   Auth token.
 * @param integer $timestamp
 *   UNI•Login timestamp.
 * @param string $unilogin_id
 *   UNI•Login user id.
 *
 * @return boolean
 *   Has the ticket been used before?
 */
function _unilogin_ticket_has_been_used($auth, $timestamp, $unilogin_id) {
  return (bool) _unilogin_db_load_ticket(
    _unilogin_construct_ticket_name($auth, $timestamp, $unilogin_id)
  );
}

/**
 * Construct concatenated ticket name.
 *
 * @param string $auth
 *   Auth token.
 * @param integer $timestamp
 *   UNI•Login timestamp.
 * @param string $unilogin_id
 *   UNI•Login user id.
 *
 * @return string
 *   Ticket name.
 */
function _unilogin_construct_ticket_name($auth, $timestamp, $unilogin_id) {
  return $auth . $timestamp . $unilogin_id;
}

/**
 * Check if timestamp is too old and therefore invalid.
 *
 * @param integer $timestamp
 *   Timestamp to be checked.
 */
function _unilogin_timestamp_is_expired($timestamp) {
  // Valid period of a timestamp.
  $minutes = variable_get(
    'unilogin_timestamp_expiration',
    UNILOGIN_DEFSET_TIMESTAMP_EXPIRATION
  );

  return strtotime($timestamp) <= strtotime(gmdate('M d Y H:i:s') . " - $minutes minutes");
}

/**
 * Verify params in unilogin callback.
 *
 * @param string $auth
 *   Auth token.
 * @param integer $timestamp
 *   UNI•Login timestamp.
 * @param string $unilogin_id
 *   UNI•Login user id.
 *
 * @return boolean
 *   Are the calback params valid?
 */
function _unilogin_verify_params($auth, $timestamp, $unilogin_id) {
  // Check if auth token is correct.
  if (!_unilogin_verify_auth($auth, $timestamp, $unilogin_id)) {
    watchdog(UNILOGIN_WD_ID, 'Auth check failed.', array(), WATCHDOG_WARNING);
    return FALSE;
  }

  // Check if ticket already has been used.
  if (_unilogin_ticket_has_been_used($auth, $timestamp, $unilogin_id)) {
    watchdog(
      UNILOGIN_WD_ID,
      'A user has already used the ticket and login has been prevented.',
      array(),
      WATCHDOG_WARNING
    );
    return FALSE;
  }

  // Check if timestamp is too old.
  if (_unilogin_timestamp_is_expired($timestamp)) {
    watchdog(
      UNILOGIN_WD_ID,
      'Timestamp has expired.',
      array(),
      WATCHDOG_WARNING
    );
    return FALSE;
  }

  // If everything passed params are valid.
  return TRUE;
}
