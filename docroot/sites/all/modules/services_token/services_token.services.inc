<?php

/**
 * @file
 * Token resource and authentication for services.
 */

/**
 * Generate a token and return the token record.
 */
function _services_token_generate() {
  if ($realm = services_token_endpoint_get_realm()) {
    return services_token_create($realm);
  }
}

/**
 * Authentication callback.
 */
function _services_token_authenticate($settings, $controller, $args) {
  // Disable page caching for any request that requires an authentication
  // check.
  drupal_page_is_cacheable(FALSE);

  // PHP FastCGI doesn't support HTTP Basic Authentication out of the box so we
  // need this workaround. Requires a patch to .htaccess.
  // @see http://drupal.org/node/1864628.
  if (
    !isset($_SERVER['PHP_AUTH_USER']) &&
    !isset($_SERVER['PHP_AUTH_PW']) &&
    isset($_SERVER['HTTP_AUTHORIZATION']) &&
    strtolower(substr($_SERVER['HTTP_AUTHORIZATION'], 0, 5)) === 'basic'
  ) {
    $credentials = base64_decode(substr($_SERVER['HTTP_AUTHORIZATION'], 6));
    if ($credentials) {
      list($_SERVER['PHP_AUTH_USER'], $_SERVER['PHP_AUTH_PW']) = explode(':', $credentials, 2);
    }
  }

  if (isset($_SERVER['PHP_AUTH_USER']) && isset($_SERVER['PHP_AUTH_PW'])) {
    $realm = services_token_endpoint_get_realm();
    if (services_token_validate($_SERVER['PHP_AUTH_USER'], $realm)) {
      list($uid,,) = services_token_split($_SERVER['PHP_AUTH_USER']);
      $account = user_load($uid);
      if ($account) {
        global $user;
        $user = $account;
        return;
      }
    }

    // If controller allows to fall back to basic auth with user/password
    // credentials, then try that here.
    if (!empty($controller['endpoint']['services_token']['password_fallback'])) {
      $form = array();
      $form_state['values'] = array(
        'name' => $_SERVER['PHP_AUTH_USER'],
        'pass' => $_SERVER['PHP_AUTH_PW'],
      );
      foreach (user_login_default_validators() as $function) {
        $function($form, $form_state);
      }
      if (!form_get_errors()) {
        global $user;
        $user = user_load($form_state['uid']);
        return;
      }
    }

    return 'Invalid credentials.';
  }
}

/**
 * Send a 401 Unauthorized challenge response if appropriate.
 *
 * @param array $error
 *   An associative array with the following keys:
 *     - 'code': the HTTP status code.
 *     - 'header_message': the message returned as part of the error response
 *       (for instance, "404 Not found").
 *     - 'body_data': data that was passed to the thrown exception.
 */
function _services_token_authenticate_send_challenge(&$error) {
  $realm = services_token_endpoint_get_realm();
  if (isset($realm)) {
    if (
      !isset($_SERVER['PHP_AUTH_USER']) &&
      !isset($_SERVER['PHP_AUTH_PW']) &&
      $error['code'] == 403 &&
      user_is_anonymous()
    ) {
      // Turn an access-denied response into an unauthorized response if there
      // are no credentials on the request.
      $error['code'] = 401;
      $error['header_message'] = '401 Unauthorized';
    }

    if ($error['code'] == 401) {
      // Make sure there is an authentication challange for 401 responses.
      drupal_add_http_header('WWW-Authenticate', 'Basic realm="' . check_plain($realm) . '"');
    }
  }
}

/**
 * Authentication settings form callback.
 */
function _services_token_authentication_settings_form($settings, &$form_state) {
  $form = array();
  $form['realm'] = array(
    '#type' => 'textfield',
    '#default_value' => $settings['realm'],
    '#title' => t('Basic Auth Realm'),
    '#description' => t('The realm to use in a Basic Auth challenge responses'),
    '#required' => TRUE,
  );
  return $form;
}

/**
 * Authentication defaults callback.
 */
function _services_token_authentication_settings_defaults() {
  return array(
    'realm' => variable_get('site_name', 'Drupal') . ' API',
  );
}

/**
 * Controller settings callback.
 *
 * Adds a checkbox to the controller settings form of the generate action which
 * allows to enable/disable the username/password fallback mechanism.
 */
function _services_token_controller_settings($settings, $controller, $endpoint, $class, $operation) {
  // Sadly the only way to determine the seleceted resource/action is to inspect
  // the controller definition :/
  if ($controller['callback'] === '_services_token_generate') {
    $form = array();
    $form['password_fallback'] = array(
      '#type' => 'checkbox',
      '#title' => t('Basic Auth Fallback'),
      '#description' => t('Allow authentication with username/password in addition to token authentication'),
      '#default_value' => !empty($settings['password_fallback']),
    );
    return $form;
  }
}
