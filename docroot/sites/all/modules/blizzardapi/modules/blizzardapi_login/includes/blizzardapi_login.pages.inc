<?php

/**
 * @file
 * User page callbacks for the Blizzard API Login module.
 */

/**
 * Menu callback; process the OAuth 2.0 authentication response.
 */
function blizzardapi_login_page() {
  if (empty($_GET['state']) || (empty($_GET['error']) && empty($_GET['code']))) {
    drupal_set_message(t('Invalid authentication request.'), 'error');
    drupal_goto();
  }
  if (!drupal_valid_token($_GET['state'], 'blizzardapi_login')) {
    drupal_set_message(t('Invalid login attempt.'), 'error');
    drupal_goto();
  }
  
  $destination = $_SESSION['blizzardapi_login']['destination'];
  $region = $_SESSION['blizzardapi_login']['region'];
  
  // RFC 6749 (section 4.1.2.1) has a full list of possible error codes.
  if (isset($_GET['error'])) {
    $message = t('Battle.net login attempt failed.');

    // Check if a description was provided.
    if (isset($_GET['error_description'])) {
      // User selected the cancel button on Blizzard's consent prompt.
      if ($_GET['error_description'] == 'User denied access') {
        $message = t('Battle.net login cancelled.');
      }
      $args = array('@error' => $_GET['error'], '@description' => $_GET['error_description']);
      watchdog('blizzardapi_login', 'Battle.net login attempt failed (@error: @description)', $args, WATCHDOG_NOTICE);
    }
    else {
      $args = array('@error' => $_GET['error']);
      watchdog('blizzardapi_login', 'Battle.net login attempt failed (@error)', $args, WATCHDOG_NOTICE);
    }
    
    drupal_set_message($message, 'error');
    drupal_goto($destination);
  }
  
  $account_id = FALSE;
  $client = blizzardapi_login_client($region);
  if (!isset($client)) {
    drupal_set_message(t('Unable to load authentication library.'), 'error');
    drupal_goto();
  }

  try {
    $client->authenticate($_GET['code']);
    $token = $client->getAccessToken();
    $account_id = $client->getAccountId();
  }
  catch (Exception $e) {
    watchdog_exception('blizzardapi_login', $e);
  }
  
  if ($account_id !== FALSE && !empty($token)) {
    blizzardapi_login_complete($account_id, $token, $destination);
  }
  
  drupal_set_message(t('Login failure.'), 'error');
  drupal_goto();
}

/**
 * Log in an existing user or send new users to the registration page.
 *
 * @param int $account_id
 *   A user's Battle.net account ID.
 * @param array $token
 *   A user's access token.
 * @param string $destination
 *   (optional) The redirect location after registration.
 */
function blizzardapi_login_complete($account_id, $token, $destination = '') {
  global $user;
  
  $account = FALSE;
  $region = $_SESSION['blizzardapi_login']['region'];
  $registration_mode = variable_get('user_register', USER_REGISTER_VISITORS_ADMINISTRATIVE_APPROVAL);
  
  $authmap = _blizzardapi_login_authmap($region, $account_id);
  if ($authmap !== FALSE) {
    $account = user_external_load($authmap);
    $expire = $token['created'] + $token['expires_in'];
    cache_set($authmap, $token, 'cache_blizzardapi_login', $expire);
  }
  
  // Existing user (logged in, attaching a Battle.net account).
  if ($user->uid) {
    if (isset($account->uid)) {
      drupal_set_message(t('That Battle.net account is already registered to another user.'), 'error');
    }
    else {
      $_SESSION['blizzardapi_login']['battlenet_id'] = $account_id;
      blizzardapi_login_set_authmap($user);
      drupal_set_message(t('You may now login using your Battle.net account.'));
    }
    unset($_SESSION['blizzardapi_login']);
    drupal_goto('user/' . $user->uid . '/edit');
  }
  // Existing user (not logged in).
  else if (isset($account->uid)) {
    if (!variable_get('user_email_verification', TRUE) || $account->login) {
      // Check if the user is blocked.
      $name_state['values'] = array('name' => $account->name);
      user_login_name_validate(array(), $name_state);
      
      // Login.
      if (!form_get_errors()) {
        $login_state['uid'] = $account->uid;
        user_login_submit(array(), $login_state);
        module_invoke_all('blizzardapi_login_success', $account, $region);
      }
    }
    else {
      drupal_set_message(t('You must validate your e-mail address for this account before logging in.'));
    }
  }
  // New user registration.
  else if ($registration_mode != USER_REGISTER_ADMINISTRATORS_ONLY) {
    // Save account information for use by our modified registration form.
    $_SESSION['blizzardapi_login']['battlenet_id'] = $account_id;
    drupal_goto('user/register', array('query' => array('destination' => $destination)));
  }
  else {
    drupal_set_message(t('Only site administrators can create new user accounts.'), 'error');
  }
  
  drupal_goto();
}

/**
 * Menu callback; process a one-time login link for Battle.net accounts.
 */
function blizzardapi_login_verify($uid, $timestamp, $hashed_pass) {
  global $user;
  
  // Make sure that a user is not already logged in.
  if ($user->uid) {
    $name = format_username($user);
    if ($user->uid == $uid) {
      // If the user is already logged in, then e-mail verification was
      // disabled, and this message is a lie.
      drupal_set_message(t('Your e-mail address has now been verified.'));
    }
    else {
      $link_account = user_load($uid);
      if (!empty($link_account)) {
        drupal_set_message(t('Another user (%other-user) is already logged into the site on this computer, but you tried to use an e-mail validation link for user %validate-user. ' .
          'Please <a href="!logout">logout</a> and try using the link again.',
          array('%other-user' => $name, '%validate-user' => format_username($link_account), '!logout' => url('user/logout'))));
      }
      else {
        drupal_set_message(t('The e-mail validation link you clicked is invalid.'));
      }
    }
    drupal_goto();
  }
  else {
    $current = REQUEST_TIME;
    $timeout = variable_get('user_password_reset_timeout', 86400);  // 24 hours.
    
    $users = user_load_multiple(array($uid), array('status' => 1));  // Not blocked.
    $account = reset($users);
    
    if ($timestamp <= $current && $account) {
      if ($account->login && $current - $timestamp > $timeout) {
        drupal_set_message(t('You have tried to use an e-mail validation link that has expired.'));
      }
      else if ($account->uid && $timestamp >= $account->login && $timestamp <= $current &&
        $hashed_pass == blizzardapi_login_rehash($account->pass, $timestamp, $account->login, $account->uid)) {
        // Change from the current anonymous user.
        $user = $account;
        user_login_finalize();
        // Log usage of this one-time link.
        watchdog('blizzardapi_login', 'User %name used a one-time login link at %timestamp.',
          array('%name' => $account->name, '%timestamp' => $timestamp));
        drupal_set_message(t('Your e-mail address has now been verified and you have automatically been logged in.'));
      }
      else {
        drupal_set_message(t('You have tried to use an e-mail validation link that has either been used or is no longer valid.'));
      }
      drupal_goto();
    }
    else {
      drupal_access_denied();
      drupal_exit();
    }
  }
}
