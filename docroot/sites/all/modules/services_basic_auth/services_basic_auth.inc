<?php

/**
 * @file
 * Include file for services_basic_auth module.
 */

/**
 * Authenticates a call using HTTP basic authentication to verify the request.
 *
 * @param array $settings
 *   The settings for the authentication module.
 * @param array $method
 *   The method that's being called.
 * @param array $args
 *   The arguments that are being used to call the method.
 *
 * @return void|string
 *   Returns nothing, or a error message if authentication fails.
 */
function _services_basic_auth_authenticate_call($settings, $method, $args) {
  // Disable page caching for any request that requires an authentication
  // check.
  drupal_page_is_cacheable(FALSE);

  // PHP FastCGI doesn't support HTTP Basic Authentication out of the box so we
  // need this workaround. Requires a patch to .htaccess.
  // @see http://drupal.org/node/1864628.
  if (isset($_SERVER['HTTP_AUTHORIZATION']) AND !empty($_SERVER['HTTP_AUTHORIZATION'])) {
    list($_SERVER['PHP_AUTH_USER'], $_SERVER['PHP_AUTH_PW']) = explode(':', base64_decode(substr($_SERVER['HTTP_AUTHORIZATION'], 6)), 2);
  }

  if (user_is_anonymous() && isset($_SERVER['PHP_AUTH_USER']) && isset($_SERVER['PHP_AUTH_PW'])) {
    // Fake a user_login form submission.
    $form_state['values'] = array(
      'name' => $_SERVER['PHP_AUTH_USER'],
      'pass' => $_SERVER['PHP_AUTH_PW'],
    );
    // Run flood check and validate login details.
    user_login_authenticate_validate(array(), $form_state);

    // Check if the user was authenticated and register flood events if
    // necessary. @see user_login_final_validate().
    if (empty($form_state['uid'])) {
      // Always register an IP-based failed login event.
      flood_register_event('failed_login_attempt_ip', variable_get('user_failed_login_ip_window', 3600));
      // Register a per-user failed login event.
      if (isset($form_state['flood_control_user_identifier'])) {
        flood_register_event('failed_login_attempt_user', variable_get('user_failed_login_user_window', 21600), $form_state['flood_control_user_identifier']);
      }
    }
    elseif (isset($form_state['flood_control_user_identifier'])) {
      // Clear past failures for this user so as not to block a user who might
      // log in and out more than once in an hour.
      flood_clear_event('failed_login_attempt_user', $form_state['flood_control_user_identifier']);

      // The user has been authenticated and has passed flood checks, so we can
      // log them in. @see user_login_submit().
      global $user;
      $user = user_load($form_state['uid']);
    }
  }
}
