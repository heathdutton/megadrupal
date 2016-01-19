<?php

/**
 * @file
 * OneLogin SAML response callback handler and associated functionality.
 */


/**
 * Menu callback to handle OneLogin SAML responses.
 *
 * @see onelogin_menu()
 */
function onelogin_consume() {
  // Check if this user already has a valid session.
  if (drupal_session_started() && $GLOBALS['user']->uid > 0) {
    watchdog('onelogin', 'Authentication attempt was made despite a pre-existing session for !user.', array(
      '!user' => l($GLOBALS['user']->mail, 'user/' . $GLOBALS['user']->uid),
    ), WATCHDOG_INFO);
  }
  // Otherwise check if a SAMLResponse was passed in with the request.
  elseif (isset($_POST['SAMLResponse']) && !empty($_POST['SAMLResponse'])) {
    // Ensure that the OneLogin SAML PHP library is loaded.
    if (($library = libraries_load('onelogin')) && !empty($library['loaded'])) {
      // Get the OneLogin settings object.
      $settings = onelogin_get_settings();

      // Create a OneLogin response object.
      $response = new OneLogin_Saml_Response($settings, $_POST['SAMLResponse']);

      // If the response is valid, attempt to authenticate the user.
      if ($response->isValid()) {
        // If authentication succeeded, ensure we redirect to the expected page.
        if (onelogin_auth($response)) {
          onelogin_auth_prepare_redirect($settings);
        }
      }
      else {
        watchdog('onelogin', 'Authentication attempt failed: an invalid SAML response was given.', array(), WATCHDOG_ERROR);
      }
    }
    else {
      watchdog('onelogin', 'Authentication attempt failed: unable to load the OneLogin SAML PHP library.', array(), WATCHDOG_ALERT);
    }
  }
  else {
    watchdog('onelogin', 'Authentication attempt failed: no SAML response was given.', array(), WATCHDOG_WARNING);
  }

  // Regardless of what happened above, redirect to the front page.
  drupal_goto();
}


/**
 * Attempts to authenticate a user, given a valid OneLogin response object.
 *
 * @param OneLogin_Saml_Response $samlresponse
 *   A valid OneLogin SAML response object.
 * @return bool
 *   TRUE if the user was successfully authenticated, otherwise FALSE.
 */
function onelogin_auth(OneLogin_Saml_Response $response) {
  $email = $response->getNameId();

  // Query for active users given an e-mail address.
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'user')
    ->propertyCondition('status', 1)
    ->propertyCondition('mail', $email);

  // If a user exists, attempt to authenticate.
  $result = $query->execute();
  if ($result && $user = user_load(key($result['user']))) {
    $GLOBALS['user'] = $user;
    $form_state['uid'] = $user->uid;
    user_login_finalize($form_state);
    return TRUE;
  }
  else {
    // If auto provisioning is enabled, create the user and log them in.
    $provision = variable_get('onelogin_autoprovision', FALSE);
    $pregmatch = variable_get('onelogin_autoprovision_email_pregmatch', '');
    $pregmatch = empty($pregmatch) ? TRUE : preg_match($pregmatch, $email);
    if ($provision && $pregmatch) {
      if (onelogin_auth_provision($response)) {
        watchdog('onelogin', 'Successfully auto-provisioned an account for %user.', array(
          '%user' => $email,
        ), WATCHDOG_INFO);
        return onelogin_auth($response);
      }
      else {
        watchdog('onelogin', 'User auto-provision failed for %user.', array(
          '%user' => $email,
        ), WATCHDOG_ERROR);
        return FALSE;
      }
    }
    else {
      watchdog('onelogin', 'Attempted to authenticate nonexistent user %user.', array(
        '%user' => $email,
      ), WATCHDOG_WARNING);
      return FALSE;
    }
  }
}


/**
 * Attempts to provision a Drupal user, given a valid OneLogin Response object.
 *
 * @param OneLogin_Saml_Response $response
 *   A valid OneLogin SAML response object.
 * @return mixed
 *   Returns the fully loaded user object upon successful provisioning, FALSE if
 *   there were any problems creating the user.
 */
function onelogin_auth_provision(OneLogin_Saml_Response $response) {
  $email = $response->getNameId();
  $fields = array(
    'name' => str_replace('@', '.', $email),
    'mail' => $response->getNameId(),
    'pass' => user_password(16),
    'status' => 1,
    'init' => 'email_address',
    'roles' => array(DRUPAL_AUTHENTICATED_RID => 'authenticated user'),
  );

  try {
    return user_save(NULL, $fields);
  }
  catch (Exception $e) {
    return FALSE;
  }
}


/**
 * Given a SAML Settings object, parses out potential destination params and if
 * one is set, sets up the global state so that the current request will be
 * redirected to the destination.
 *
 * Note: this is secure because drupal_goto will refuse to redirect to an
 * external URL.
 *
 * @param OneLogin_Saml_Settings $settings
 *   The onelogin saml settings object.
 *
 * @see onelogin_get_settings()
 * @see drupal_goto()
 * @see onelogin_consume()
 */
function onelogin_auth_prepare_redirect(OneLogin_Saml_Settings $settings) {
  $redirect = drupal_parse_url($settings->spReturnUrl);
  if (isset($redirect['query']['destination'])) {
    $_GET['destination'] = $redirect['query']['destination'];
  }
}


/**
 * Creates and returns a OneLogin settings object.
 *
 * @return OneLogin_Saml_Settings
 *   A fully loaded and configured OneLogin_Saml_Settings object.
 */
function onelogin_get_settings() {
  if (class_exists('OneLogin_Saml_Settings')) {
    $urlOptions = array(
      'absolute' => TRUE,
    );

    // @see onelogin_require_authentication()
    if (isset($_SERVER['HTTP_REFERER']) && !empty($_SERVER['HTTP_REFERER'])) {
      $referer = drupal_parse_url($_SERVER['HTTP_REFERER']);
      if (isset($referer['query']['originalRefererPath'])) {
        $urlOptions['query'] = array(
          'destination' => $referer['query']['originalRefererPath'],
        );
      }
    }

    $settings = new OneLogin_Saml_Settings();
    $settings->idpSingleSignOnUrl = variable_get('onelogin_login_url', '');
    $settings->idpPublicCertificate = variable_get('onelogin_cert', '');
    $settings->spReturnUrl = url('onelogin_saml/consumer', $urlOptions);
    $settings->spIssuer = 'onelogin_saml';
    return $settings;
  }
  else {
    return NULL;
  }
}
