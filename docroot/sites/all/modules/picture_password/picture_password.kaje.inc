<?php

/**
 * @file
 * picture_password.kaje.in
 *
 * Single API function for KAJE. One function to do it all.
 * 
 * This API does not use drupal_set_message() or watchdog(). All messages and
 * errors are communicated via a response object that contains both a msg_code
 * and a msg string.
 */

define('PICTURE_PASSWORD_KAJE_BASE_URL', 'https://kaje.authenticator.com/index.php/auth');

/**
 * Main Kaje API function.
 *
 * @param string $type
 *   One of 'status' 'request', 'newUser', 'deleteUser' 'id', 'reset', 'lock',
 *   'unlock', 'suspend', 'unsuspend'
 * @param string $kaje_id
 *   NOT required for 'status', 'id' and 'newUser' calls.
 * @param string $auth_token
 *   Required for the 'id' call only.
 *
 * @return object
 *   Response object containing at the minimum msg_code and msg fields.
 *
 *   The response object contains info related to the API call issued.
 *   Reasons why this function may return error msg_codes:
 *    o unsupported call $type or other Kaje server error (msg_code > 0)
 *    o rp_id and/or rp_secret missing (msg_code -200)
 *    o problems with the CURL command used to connect to Kaje (-101, -100)
 *    o rp_id (from config page) does not match that returned by Kaje (-1)
 *    o $kaje_id, where required, does not match that returned by Kaje (-2)
 *    o auth_token, where required, does not match that returned by Kaje (-3)
 *
 * This a synchronous call, so executing halts inside this function until a
 * response is received or a time-out occurs.
 */
function picture_password_kaje_call($type, $kaje_id = NULL, $auth_token = NULL) {

  // Add call type and timestamp to the base URL.
  $url = PICTURE_PASSWORD_KAJE_BASE_URL . "/$type?ts=" . time();

  $rp_id = variable_get('picture_password_rp_id');
  $rp_secret = variable_get('picture_password_rp_secret');

  if ($type != 'status' && (empty($rp_id) || empty($rp_secret))) {
    $msg = t('Requesting Party ID or Secret not set. Your site administrator needs to set these on the Kaje™ Picture Passwords <a href="@url_config">configuration page</a>.', array(
      '@url_config' => url('admin/config/system/picture-password')));
    return _picture_password_kaje_return($msg, -200);
  }

  // Add POST info based on the type of API call.
  $post_data = array('rp_id' => $rp_id, 'rp_secret' => $rp_secret);
  switch ($type) {
    case 'status':
      break;

    case 'id':
      $post_data['auth_token'] = $auth_token;
      break;

    default:
      $post_data['user_id'] = $kaje_id;
      break;
  }
  $ca_cert_file = drupal_get_path('module', 'picture_password') . '/ca/cacert.pem';

  $ch = curl_init();
  if (!$ch) {
    $msg = t('Kaje™ Picture Passwords: could not initialize CURL command. %error', array(
      '%error' => curl_error())
    );
    return _picture_password_kaje_return($msg, -101);
  }
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
  curl_setopt($ch, CURLOPT_HEADER, FALSE);
  curl_setopt($ch, CURLOPT_FOLLOWLOCATION, TRUE);
  curl_setopt($ch, CURLOPT_URL, $url);
  curl_setopt($ch, CURLOPT_MAXREDIRS, 10);
  curl_setopt($ch, CURLOPT_USERAGENT, 'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.52 Safari/537.17');
  curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 2);
  curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, TRUE);
  curl_setopt($ch, CURLOPT_POST, TRUE);
  curl_setopt($ch, CURLOPT_POSTFIELDS, $post_data);
  curl_setopt($ch, CURLOPT_CAINFO, $ca_cert_file);

  $output = curl_exec($ch);
  if (!$output) {
    $msg = t('Kaje™ Picture Passwords: error while executing CURL command. %error. Are you connected to the Internet?', array(
      '%error' => curl_error($ch))
    );
    curl_close($ch);
    return _picture_password_kaje_return($msg, -100);
  }
  curl_close($ch);

  $response = json_decode($output);

  if (picture_password_is_debug()) {
    drupal_set_message(t('DEBUG: executed Kaje command %cmd.', array('%cmd' => $type)));
    drupal_set_message(t('DEBUG: response') . ': ' . print_r($response, TRUE));
  }

  if (!$response) {
    return _picture_password_kaje_return($response, -1);
  }

  // rp_id, kaje_id and auth_token (if supplied) must all match what came back
  // from Kaje.
  if ($type != 'status') {
    if (!isset($response->rp_id) || $response->rp_id != $post_data['rp_id']) {
      return _picture_password_kaje_return($response, -2);
    }
  }
  if ($kaje_id) {
    if (!isset($response->user_id) || $response->user_id != $post_data['user_id']) {
      return _picture_password_kaje_return($response, -3);
    }
  }
  if ($auth_token) {
    if (!isset($response->auth_token) || $response->auth_token != $post_data['auth_token']) {
      return _picture_password_kaje_return($response, -4);
    }
  }
  return _picture_password_kaje_return($response);
}

/**
 * Returns a response object with at the minimum a msg_code and msg string.
 *
 * @param mixed $response_or_msg
 *   Response object or message string.
 * @param int $msg_code
 *   Integer message code. May be negative.
 *
 * @return object
 *   The Kaje AP response object.
 */
function _picture_password_kaje_return($response_or_msg, $msg_code = NULL) {
  $response = is_object($response_or_msg) ? $response_or_msg : new stdClass();
  if (isset($msg_code)) {
    $response->msg_code = $msg_code;
  }
  if (is_string($response_or_msg)) {
    $response->msg = $response_or_msg;
  }
  else {
    $response->msg = picture_password_kaje_msg(is_object($response_or_msg) ? $response_or_msg->msg_code : $msg_code);
  }
  return $response;
}

/**
 * Return the message text associated with the supplied message code.
 *
 * @param int $msg_code
 *   Message code. Maybe negative.
 * @param string $msg_prefix
 *   Prefix to be prepended to all messages.
 *
 * @return string
 *   The message text belonging to the supplied message code.
 */
function picture_password_kaje_msg($msg_code, $msg_prefix = '') {
  $message = $msg_prefix;

  switch ($msg_code) {
    case 0:
      return '';

    case -1:
      $message .= t('Third party server returned 404: page not found.');
      break;

    case -2:
      $message .= t("Supplied Responding Party ID does not match Kaje's.");
      break;

    case -3:
      $message .= t("Supplied user id does not match Kaje's.");
      break;

    case -4:
      $message .= t("Supplied authorization token does not match Kaje's.");
      break;

    case '100':
      $message .= t('Requesting Party ID not found.');
      break;

    case '101':
      $message .= t('Requesting Party Secret invalid.');
      break;

    case '102':
      $message .= t('Requesting Party Domain Not Verified.');
      break;

    case '103':
      $message .= t('Requesting Party has no available POKs (proofs of knowledge).');
      break;

    case '104':
      $message .= t('Requesting Party Account is locked or suspended.');
      break;

    case '200':
      $message .= t('Error creating user.');
      break;

    case '201':
      $message .= t('Kaje user account ID not found.');
      break;

    case '202':
      $message .= t('Kaje user account locked.');
      break;

    case '203':
      $message .= t('Kaje user account suspended.');
      break;

    case '204':
      $message .= t('Kaje user account deleted.');
      break;

    case '205':
      $message .= t('Kaje user reached maximum authorizations per minute.');
      break;

    case '206':
      $message .= t('Kaje user reached maximum authorizations per hour.');
      break;

    case '207':
      $message .= t('Kaje user reached maximum authorizations per day.');
      break;

    case '300':
      $message .= t('Succesfully authenticated via Kaje.');
      break;

    case '400':
      $message .= t('Kaje auth token not found.');
      break;

    case '401':
      $message .= t('Kaje auth token expired.');
      break;

    case '402':
      $message .= t('Error retrieving Kaje ID token.');
      break;

    case '500':
      $message .= t('Requesting Party is not an affiliate.');
      break;

    case '501':
      $message .= t('Discount SSL location invalid.');
      break;

    case '502':
      $message .= t('Discount percentage invalid.');
      break;

    case '503':
      $message .= t('Discount expiration invalid.');
      break;

    case '504':
      $message .= t('Error creating discount.');
      break;

    case '700':
      $message .= t('Kaje service available.');
      break;

    case '800':
      $message .= t('Kaje service unavailable.');
      break;

    case '801':
      $message .= t('Kaje service down for maintenance.');
      break;

    case '900':
    default:
      $message .= t('Unknown message code: %code.', array('%code' => $msg_code));
  }
  return $message;
}

/**
 * Returns the status string associated with a supplied user status code.
 *
 * @param string $user_status
 *   User status string code as optained from the Kaje API function response.
 * @param string $msg_prefix
 *   An opitonal prefix to be prepended to all status strings.
 *
 * @return string
 *   The status string corresponding to the supplied user status code.
 */
function picture_password_user_status($user_status, $msg_prefix = '') {
  $status_message = $msg_prefix;

  switch ($user_status) {
    case 'new':
      $status_message .= t('Your Kaje account has been created.');
      break;

    case 'reset':
      $status_message .= t('Kaje account has been RESET.');
      break;

    case 'suspended':
    case 'account currently suspended':
      $status_message .= t('Kaje account is SUSPENDED.');
      break;

    case 'unsuspended':
      $status_message .= t('Kaje account is UNSUSPENDED.');
      break;

    case 'locked':
      $status_message .= t('Kaje account is LOCKED.');
      break;

    case 'unlocked':
      $status_message .= t('Kaje account is UNLOCKED.');
      break;

    case 'nonexistent':
      $status_message .= t('Kaje account no longer exists.');
      break;

    default:
      $status_message = $user_status;
  }
  return $status_message;
}
