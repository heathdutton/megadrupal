<?php
/**
 * @file
 * Service definitions for Janrain Module in login mode
 */

/**
 * Implements hook_services_resources() for janrain_services_resources().
 */
function _janrain_services_resources() {
  return array(
    'login' => array(
      'actions' => array(
        'token' => array(
          'help' => 'Push Janrain identity credentials into user session',
          'callback' => '_janrain_login_token_service_callback',
          'args' => array(
            array(
              'name' => 'token',
              'type' => 'string',
              'description' => 'a Janrain Social Login access token',
              'source' => array('data' => 'token'),
              'optional' => FALSE,
            ),
          ),
          'access callback' => 'user_is_anonymous',
          'file' => array('file' => 'includes/login_services.php', 'module' => 'janrain'),
        ),
      ),
    ),
  );
}

/**
 * Handles login/token.
 */
function _janrain_login_token_service_callback($token) {
  $sdk = JanrainSdk::instance();

  try {
    // Trade token for profile.
    $profile = $sdk->EngageApi->fetchProfileByToken($token);
  }
  catch (Exception $e) {
    // Engage call failed for login, this is superbad.
    watchdog('janrain', $e->getMessage(), array(), WATCHDOG_EMERGENCY);
    // Also log the full stack dump to syslog.
    error_log($e->getTraceAsString());
    // Services_error rethrows.
    services_error('Invalid token!');
  }

  // Notify listeners.
  module_invoke_all("janrain_profile_received", $profile);

  // Set session data.
  DrupalAdapter::setSessionItem('identifiers', $profile->getIdentifiers());
  DrupalAdapter::setSessionItem('profile', $profile->__toString());
  return 'Session enhanced with social login data! Proceed to login';
}
