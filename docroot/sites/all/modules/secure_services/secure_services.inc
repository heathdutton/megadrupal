<?php

/**
 * @file
 * Include file for secure_services module.
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
function _secure_services_authenticate_call($settings, $method, $args) {
  // Check to see if $user has authenticate.
  $authenticateuid = user_authenticate($_SERVER['PHP_AUTH_USER'], $_SERVER['PHP_AUTH_PW']);
  $userauthenticate = user_load($authenticateuid);
  if (user_access('set permission in service basic auth', $userauthenticate)) {
    $user = $userauthenticate->name;
    $pass = $userauthenticate->pass;
    if (!empty($_SERVER['PHP_AUTH_USER']) && isset($_SERVER['PHP_AUTH_PW'])
      && $_SERVER['PHP_AUTH_USER'] == $user
      && $_SERVER['PHP_AUTH_PW'] == $pass
    ) {
      return;
    }
  }

  if (!(isset($_SERVER['PHP_AUTH_USER']) && ($_SERVER['PHP_AUTH_USER'] == $user && isset($_SERVER['PHP_AUTH_PW']) == $pass))) {
    header('WWW-Authenticate: Basic realm="This site is protected"');
    header('HTTP/1.0 401 Unauthorized');
    // Fallback message when the user presses cancel / escape.
    echo 'Access denied';
    exit;
  }
}
