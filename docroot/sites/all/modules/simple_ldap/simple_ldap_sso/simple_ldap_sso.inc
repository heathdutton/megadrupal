<?php

/**
 * @file
 * Simple LDAP SSO API functions.
 */

define('SIMPLE_LDAP_SSO_COOKIE', 'SLDAPSSO');
define('SIMPLE_LDAP_SSO_FLOOD', 'simple_ldap_sso_failed_login');

/**
 * Check to make sure this module is configured properly.
 */
function simple_ldap_sso_configured() {
  $configured = &drupal_static(__FUNCTION__);
  if (!isset($configured)) {
    // If there are no error messages, the module is configured.
    $errors = simple_ldap_configuration_errors();
    $configured = empty($errors);
  }
  return $configured;
}

/**
 * Return an array of not configuration errors.
 *
 * For use in hook_requirements() and simple_ldap_sso_configured().
 */
function simple_ldap_configuration_errors() {
  $errors = array();

  // We need the module files here in order to call these functions.
  drupal_load('module', 'simple_ldap');
  drupal_load('module', 'simple_ldap_user');
  /*
  if (!simple_ldap_configured() || !simple_ldap_user_configured()) {
    $errors = t('Simple LDAP and Simple LDAP User modules must be configured.');
  }
   */

  // Ensure the encryption key is set.
  if (!variable_get('simple_ldap_sso_encryption_key')) {
    $errors[] = t('You must specify an encryption key.');
  }

  // Check the readonly variable.
  if (variable_get('simple_ldap_readonly')
    && (!variable_get('simple_ldap_sso_binddn') ||
        !variable_get('simple_ldap_sso_bindpw'))
  ) {
    $errors[] = t('This site is in read only mode, so you must specify separate LDAP credentials with read/write access.');
  }

  // Ensure the attribute sid is set.
  if (!variable_get('simple_ldap_sso_attribute_sid')) {
    $errors[] = t('You must specify the LDAP attribute used to store the hashed session id.');
  }

  // Ensure the session_inc variable is set properly. We can't use
  // drupal_get_path here, because it's not loaded yet when this gets called
  // from the session include. Granted, it wouldn't be called from the session
  // include unless this particular bit was configured properly, but we leave
  // this in so that it works for hook_requirements().
  $session_inc_path = __DIR__ . '/simple_ldap_sso.session.inc';
  if (DRUPAL_ROOT . '/' . variable_get('session_inc') != $session_inc_path) {
    $errors[] = t('You must set the session_inc variable to Simple LDAP SSO’s session include file.');
  }

  // Ensure mcrypt is available.
  if (!extension_loaded('mcrypt')) {
    $t_args['!url'] = 'http://php.net/mcrypt';
    $errors[] = t('You must have the <a href="!url">PHP mcrypt extension</a> installed.', $t_args);
  }

  return $errors;
}

/**
 * Detects an existing session from another site.
 */
function simple_ldap_sso_detect_sid() {
  // First, check that the module is configured.
  if (simple_ldap_sso_configured()
    // Check to make sure there is a session ID.
    && isset($_COOKIE[session_name()])
    // And a SSO cookie.
    && isset($_COOKIE[SIMPLE_LDAP_SSO_COOKIE])
    // And that there isn't an existing session already on this site.
    && !simple_ldap_sso_session_exists()) {
      // Then attempt to populate the session based on the SSO Cookie. If this
      // fails, delete the cookie.
      if (!simple_ldap_sso_populate_session()) {
        simple_ldap_sso_delete_cookie();
        $message = 'Possible break-in attempt detected.';
        $t_args = array();
        watchdog(__FUNCTION__, $message, $t_args, WATCHDOG_ALERT);
      }
    }
}

/**
 * Determine if a session already exists on this site.
 */
function simple_ldap_sso_session_exists() {
  global $is_https;
  $sid = $_COOKIE[session_name()];

  $sql = $is_https ? "SELECT uid FROM {sessions} WHERE ssid = :sid" :
                     "SELECT uid FROM {sessions} WHERE sid = :sid";

  return db_query($sql, array(':sid' => $sid))->fetchField();
}

/**
 * Helper function to decrypt the SSO cookie, and verify its data.
 *
 * @return bool
 *   TRUE if a session id was found and populated. FALSE otherwise.
 */
function simple_ldap_sso_populate_session() {
  // The SSO cookie is trusted implicitly here if it can be decoded. It is then
  // checked in hook_init against the LDAP stored value during hook_init(). If
  // it is found to not be valid there, the session we create here is destroyed,
  // a watchdog error is logged, and the user is logged out.
  if ($data = simple_ldap_sso_get_cookie_data()) {
    $key = array(
      'sid' => $data['sid'],
      'ssid' => $data['ssid'],
    );

    if ($data['uid'] == 1) {
      // Do nothing for user 1.
      return FALSE;
    }

    // Unset the name field, as we don't need that here.
    unset($data['name']);

    // Empty out the session data.
    $data['session'] = '';

    $query = db_merge('sessions')
      ->key($key)
      ->fields($data);

    return $query->execute();
  }
  return FALSE;
}

/**
 * Read the SSO data from the cookie.
 *
 * @param string $cookie_value
 *   Optional cookie value to read the data from. If left blank, the cookie will
 *   be read from $_COOKIE. This argument likely will only be used during
 *   testing.
 *
 * @return mixed
 *   An array of data, or FALSE if it was unable to be read.
 */
function simple_ldap_sso_get_cookie_data($cookie_value = NULL) {
  $result = &drupal_static(__FUNCTION__);

  // If this has already been called, return what we have.
  if (isset($result)) {
    return $result;
  }

  // Check to make sure the cookie exists.
  if (!isset($cookie_value)) {
    if (!isset($_COOKIE[SIMPLE_LDAP_SSO_COOKIE])) {
      $result = FALSE;
      return $result;
    }
    $cookie_value = $_COOKIE[SIMPLE_LDAP_SSO_COOKIE];
  }
  $result = simple_ldap_sso_decrypt($cookie_value);

  // Validate the user exists on this site, by looking up the uid.
  if ($result && $uid = simple_ldap_sso_validate_user($result)) {
    $result['uid'] = $uid;
  }
  else {
    $result = FALSE;
  }

  return $result;
}

/**
 * Validate the user exists, and the data is valid.
 */
function simple_ldap_sso_validate_user(array $data) {
  // Check to see if the user exists.
  $result = db_query("SELECT uid FROM {users} WHERE name = :name", array(':name' => $data['name']));
  $uid = $result->fetchField();

  // If we don't have a $uid, create the user.
  if (!$uid) {
    // We can't call user_save() in here, because it's too early in the
    // bootstrap. Instead, we'll just write the record to the user table
    // directly.
    require_once DRUPAL_ROOT . '/includes/common.inc';
    $account = new stdClass();
    $account->uid = db_next_id(db_query('SELECT MAX(uid) FROM {users}')->fetchField());
    $account->created = REQUEST_TIME;
    $account->name = $data['name'];
    $account->status = 1;
    drupal_write_record('users', $account);
    $uid = isset($account->uid) ? $account->uid : FALSE;
  }

  return $uid ? $uid : FALSE;
}

/**
 * Validate the SSO data matches what we have in LDAP.
 */
function simple_ldap_sso_validate_ldap() {
  global $is_https;

  $sid_key = $is_https ? 'ssid' : 'sid';

  $cookie_data = simple_ldap_sso_get_cookie_data();
  if (!$cookie_data || !isset($cookie_data[$sid_key])) {
    return FALSE;
  }

  $sso = SimpleLdapSSO::singleton($cookie_data['name']);
  return $sso->validateSid($cookie_data[$sid_key]);
}

/**
 * Encrypt an array of session data.
 */
function simple_ldap_sso_encrypt(array $data) {
  $text = serialize($data);
  return _simple_ldap_encrypt_decrypt($text, TRUE);
}

/**
 * Decrypt a string to an array of session data.
 */
function simple_ldap_sso_decrypt($string) {
  $serialized = _simple_ldap_encrypt_decrypt($string, FALSE);
  if ($serialized && $data = @unserialize($serialized)) {
    return $data;
  }
  return FALSE;
}

/**
 * Helper function to encrypt or decrypt data.
 *
 * @param string $text
 *   A string of text to encrypt or decrypt.
 * @param bool $encrypt
 *   TRUE if encrypting, FALSE if decrypting.
 *
 * @return string
 *   A string of encrypted or decrypted text.
 */
function _simple_ldap_encrypt_decrypt($text, $encrypt = TRUE) {
  // Hash the key to get a more secure key.
  $key = hash('sha256', variable_get('simple_ldap_sso_encryption_key'), TRUE);
  $cipher = 'rijndael-256';
  $mode = MCRYPT_MODE_CBC;
  $iv_size = mcrypt_get_iv_size($cipher, $mode);

  if ($encrypt) {
    // Create the IV.
    $iv = mcrypt_create_iv($iv_size, MCRYPT_RAND);
    // Generate an HMAC.
    $hmac = hash('sha256', trim($text));
    // Prepend the text with the HMAC.
    $text = $hmac . $text;
    // Now encrypt the text.
    $encrypted_text = mcrypt_encrypt($cipher, $key, $text, $mode, $iv);
    // Prepend the encrypted text with the IV before it is base64 encoded.
    $output = base64_encode($iv . $encrypted_text);
  }
  else {
    // Decode the string.
    $decoded = base64_decode($text);
    // Get the IV from the beginning of the string.
    $iv = substr($decoded, 0, $iv_size);
    // Get the encrypted data from the string.
    $data = substr($decoded, $iv_size);
    // Now, decrypt the string.
    $output = mcrypt_decrypt($cipher, $key, $data, $mode, $iv);
    // Get the HMAC from the front of this string.
    $hmac = substr($output, 0, 64);
    // Remove the HMAC from the output.
    $output = substr($output, 64);
    // If the hash doesn't match, log an error and return an empty string.
    if ($hmac != hash('sha256', trim($output))) {
      $message = 'Possible break-in attempted. The HMAC does not match on the encrypted text.';
      watchdog(__FUNCTION__, $message, array(), WATCHDOG_ALERT);
      $output = '';
    }
  }

  return $output;
}

/**
 * Set the SSO cookie.
 */
function simple_ldap_sso_set_cookie(array $data) {
  $value = simple_ldap_sso_encrypt($data);
  $params = session_get_cookie_params();
  $expire = $params['lifetime'] ? REQUEST_TIME + $params['lifetime'] : 0;

  setcookie(
    SIMPLE_LDAP_SSO_COOKIE,
    $value,
    $expire,
    $params['path'],
    $params['domain'],
    (bool) variable_get('https'),
    $params['httponly']
  );
}

/**
 * Delete the SSO cookie.
 */
function simple_ldap_sso_delete_cookie() {
  $params = session_get_cookie_params();
  setcookie(
    SIMPLE_LDAP_SSO_COOKIE,
    '',
    REQUEST_TIME - 3600,
    $params['path'],
    $params['domain'],
    (bool) variable_get('https'),
    $params['httponly']
  );
}

/**
 * Save the session id to LDAP.
 */
function simple_ldap_sso_ldap_save_sid($name, $sid) {
  $sso = SimpleLdapSSO::singleton($name);
  $sso->saveSid($sid);
}

/**
 * Delete the session id from LDAP.
 */
function simple_ldap_sso_ldap_delete_sid($name) {
  $sso = SimpleLdapSSO::singleton($name);
  try {
    $sso->deleteSid();
  }
  catch (Exception $e) {
    $message = 'Unable to delete sid from LDAP for user %name. Error: @e';
    $t_args = array('%name' => $name, '@e' => (string) $e);
    watchdog(__FUNCTION__, $message, $t_args, WATCHDOG_WARNING);
  }
}

/**
 * Abort the SSO. Logs a watchdog alert, and destroys the session.
 */
function simple_ldap_sso_abort() {
  global $user;
  $message = 'Possible break-in attempt detected for uid @uid and session id @id.';
  $t_args = array(
    '@uid' => $user->uid,
    '@id' => session_id(),
  );
  watchdog(__FUNCTION__, $message, $t_args, WATCHDOG_ALERT);
  // Remove the SSO cookie and delete the session data from LDAP.
  simple_ldap_sso_user_logout($user);
  // Log out the user and reset the $user object.
  session_destroy();
  drupal_set_message(t('A problem was encountered when attempting to sign you in on this site.'), 'error');
}

/**
 * Determine if an SSO session has been validated yet.
 */
function simple_ldap_sso_does_session_need_validation() {
  // If the variable isn't set in the session, then it needs to be validated.
  // Otherwise, just return the result of the session variable.
  return !isset($_SESSION['simple_ldap_sso_needs_validation'])
    || $_SESSION['simple_ldap_sso_needs_validation'];
}

/**
 * Mark a session as valid.
 */
function simple_ldap_sso_session_is_valid() {
  $_SESSION['simple_ldap_sso_needs_validation'] = FALSE;
}

/**
 * Mark a session as needing validation.
 */
function simple_ldap_sso_session_needs_validation() {
  $_SESSION['simple_ldap_sso_needs_validation'] = TRUE;
}
