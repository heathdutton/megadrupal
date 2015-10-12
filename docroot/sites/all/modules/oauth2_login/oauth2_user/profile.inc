<?php
/**
 * @file
 * OAuth2 user profile.
 */

/**
 * Get the profile of the oauth2 user.
 */
function oauth2_user_get() {
  // If it is already cached, return the cached one.
  if (isset($_SESSION['oauth2_user']['profile'])) {
    return $_SESSION['oauth2_user']['profile'];
  }
  else {
    return oauth2_user_get_from_server();
  }
}

/**
 * Get the profile of the oauth2 user from the server.
 */
function oauth2_user_get_from_server() {
  if (!oauth2_user_is_authenticated())  return NULL;

  $oauth2 = wsclient_service_load('oauth2');
  $oauth2_user = $oauth2->user_profile();

  oauth2_user_save($oauth2_user);
  return $_SESSION['oauth2_user']['profile'];
}

/**
 * Save the profile of the user on session.
 */
function oauth2_user_save($oauth2_user) {
  // Allow other modules to customize the oauth2 user profile.
  drupal_alter('oauth2_user', $oauth2_user);
  $_SESSION['oauth2_user']['profile'] = $oauth2_user;
}

/**
 * Remove oauth2_user from the session.
 * This will cause the profile to be refreshed from the server.
 */
function oauth2_user_expire() {
  unset($_SESSION['oauth2_user']['profile']);
}

/**
 * Implements hook_hybridauth_provider_config_alter().
 */
function oauth2_user_hybridauth_provider_config_alter(&$config, $provider_id) {
  if ($provider_id != 'DrupalOAuth2') {
    return;
  }

  // Allow other modules to specify the fields of the user profile.
  // These fields can be defined by the oauth2 server
  // on hook_oauth2_loginprovider_userprofile_alter().
  $profile_fields = array();
  drupal_alter('oauth2_profile_fields', $profile_fields);

  // Add extra profile fields to the config.
  foreach ($profile_fields as $field) {
    $config['profile_fields'][$field] = $field;
  }
}

/**
 * Implements hook_hybridauth_user_login().
 */
function oauth2_user_hybridauth_user_login($account, $oauth2_user_data) {
  oauth2_user_save($oauth2_user_data);
}
