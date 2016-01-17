<?php

/**
 * Authenticate a call.
 * 
 * If call is authenticated, load the user owning the key and replace user roles
 * by the roles assigned to the key.
 * 
 * @param unknown_type $settings
 * @param unknown_type $method
 * @param unknown_type $args
 */
function _lauth_service_authenticate_call($settings, $method, $args) {
  $cred = isset($endpoint['credentials']) ? $endpoint['credentials'] : 'token'; // FIXME what does this mean?
  // If no credentials are needed we'll pass this one through
  if ($cred == 'none') {
    return FALSE;
  }

  $verify_result = lauth_server_verify_request();
  
  if ($verify_result['success'] === TRUE) {
    global $user;
    $user = user_load($verify_result['uid']);
    
    watchdog('lauth_service', 'Session opened for %name using lauth key %key.', array('%name' => $user->name, '%key' => $verify_result['key']));
    
    $user->lauth_key_id = $verify_result['key'];
    $user->login = REQUEST_TIME;
    $user->roles = $verify_result['roles'];
    $user->roles[DRUPAL_AUTHENTICATED_RID] = 'authenticated user';
    
    // Regenerate the session ID to prevent against session fixation attacks.
    // This is called before hook_user in case one of those functions fails
    // or incorrectly does a redirect which would leave the old session in place.
    drupal_session_regenerate();
    
    $edit = array();
    user_module_invoke('login', $edit, $user);
  }
  else {
    drupal_add_http_header('WWW-Authenticate', sprintf('LAuth realm="%s"', url('', array('absolute' => TRUE))));
    return $verify_result['error'];
  }
}

function _lauth_service_security_settings($settings) {
  return array();
}

function _lauth_service_controller_settings($settings, $controller, $endpoint, $class, $name) {
  return array();
}
