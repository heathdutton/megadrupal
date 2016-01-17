<?php

/**
 * Callback function for facebook_rules_generate_token.
 */
function facebook_rules_generate_token($app_id, $access_token) {
  global $user;
  $uid = $user->uid;

  // Store token.
  facebook_rules_store_token($access_token, $uid);
  // Change the value of the variable.
  facebook_rules_set_token_status($uid, '0');
  // Notify user of token generation.
  drupal_set_message(t("Your facebook token has been generated."), 'status');

  // In case user generates access token immediately
  // his failed update would post to facebook.

  if (isset($_SESSION['facebook_rules-' . $GLOBALS['user']->uid . '-failed_update-url'])) {
    $url = $_SESSION['facebook_rules-' . $GLOBALS['user']->uid . '-failed_update-url'];
    $data = $_SESSION['facebook_rules-' . $GLOBALS['user']->uid . '-failed_update-message'];
    $data['access_token'] = $access_token;
    facebook_rules($url, $data);
  }
}

/**
 * Store access token for the user.
 */
function facebook_rules_store_token($token, $uid) {

  db_merge('facebook_rules_user_tokens')
    ->key(array('uid' => $uid))
    ->fields(array(
      'token' => $token,
      'timestamp' => time(),
    ))
    ->execute();

}

/**
 * Remove access token for the user.
 */
function facebook_rules_remove_token($uid) {

  db_delete('facebook_rules_user_tokens')
  ->condition('uid', $uid)
  ->execute();

}

/**
 * Get access token for the user.
 */
function facebook_rules_get_token($uid) {
  return db_query("SELECT token FROM {facebook_rules_user_tokens} WHERE uid = :uid", array(':uid' => $uid))->fetchField();
}

/**
 * This function makes the http request to post data on facebook.
 */
function facebook_rules_post($url, $data) {

  $response = drupal_http_request($url, array('data' => http_build_query($data, '', '&'), 'method' => 'POST'));
  $content = json_decode($response->data, TRUE);

  return $content;
}

/**
 * Set access token status for the user.
 */
function facebook_rules_set_token_status($uid, $value) {

    db_update('facebook_rules_user_tokens')
      ->fields(array(
        'token_status' => $value,
      ))
      ->condition('uid', $uid, '=')
      ->execute();
}

/**
 * Get access token status for the user.
 */
function facebook_rules_get_token_status($uid) {
  return db_query("SELECT token_status FROM {facebook_rules_user_tokens} WHERE uid = :uid", array(':uid' => $uid))->fetchField();
}

/**
 * Check if user has allowed posting to facebook on his behalf.
 */
function facebook_rules_profile_user_grant($uid) {
  return db_query("SELECT user_grant FROM {facebook_rules_user_tokens} WHERE uid = :uid", array(':uid' => $uid))->fetchField();
}

/**
 * Renew existing valid access token for the user.
 * This is done to extend the validity period of the token.
 */
function facebook_rules_renew_token($uid) {
  $current_token = facebook_rules_get_token($uid);
    if (!$current_token) {
      return;
    }
  $app_id = variable_get('fboauth_id', '');
  $app_secret = variable_get('fboauth_secret', '');
  
  $token_url = "https://graph.facebook.com/oauth/access_token?client_id=$app_id&client_secret=$app_secret&grant_type=fb_exchange_token&fb_exchange_token=$current_token";
  $token_request = drupal_http_request($token_url);
  
  if ($token_request->code == '200') {
  parse_str($token_request->data, $new_token);
  
    db_merge('facebook_rules_user_tokens')
      ->key(array('uid' => $uid))
      ->fields(array(
        'token' => $new_token['access_token'],
        'timestamp' => time(),
      ))
      ->execute();
  }
}

