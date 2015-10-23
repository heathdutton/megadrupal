<?php

/**
 * @file
 * General functionality for a Single Sign-On
 */

/**
 * Implements hook_form_alter().
 *
 * - Keep destinations spanning different sessions.
 * - Simplify openid_provider_form.
 *
 * @todo Support non-clean URLs in redirects.
 */
function openid_sso_provider_form_alter(&$form, &$form_state, $form_id) {

  // Add redirects to Relying Party if there is a given realm.
  // On user_pass_reset stick redirect into session variable and redirect on
  // submission of user_profile_form form - this is where user_pass_reset
  // redirects to. Otherwise simply add redirect to $form variable.
  // When a destination is set, make sure it is kept on certain forms so that
  // the user always winds up at the RP where she started out.
  // The fundamental difference between the destination 'destination' and the
  // destination 'realm' is that 'destination' is an internal destination of
  // this site, while 'realm' is a relying party that requested authentication.
  $realm = isset($_GET['realm'])? $_GET['realm'] : NULL;
  if ($form_id == 'user_pass_reset' && $rp = openid_sso_provider_rps_load_by_realm($realm)) {
    $_SESSION['openid_sso_provider_redirect'] = $rp->realm . 'sso/init';
  }
  elseif ($form_id == 'user_profile_form') {
    $form['#submit'][] = 'openid_sso_provider_user_submit';
  }
  elseif (in_array($form_id, array('user_register', 'user_login', 'user_pass'))) {
    if (isset($_GET['destination']) && $_GET['destination'] == 'openid/provider/continue') {
      drupal_add_js(drupal_get_path('module', 'openid_sso_provider') . '/openid_sso_provider.js');
      drupal_add_js('Drupal.settings.openid_sso_provider_destination=' . drupal_json_encode($_GET['destination']) . ';', array('type' => 'inline', 'scope' => JS_DEFAULT));
    }
    // FIXME: where is this realm parameter supposed to be set??
    if (isset($_GET['realm']) && $rp = openid_sso_provider_rps_load_by_realm($_GET['realm'])) {
      $form['#submit'][] = 'openid_sso_provider_user_submit';
      $_SESSION['openid_sso_provider_redirect'] = $rp->realm . 'sso/init';
    }
  }
  // If RP is known take decisions that simplify the user's choices.
  // Otherwise deny access. This only denies new RPs access. If there has been
  // an RP in the past that has been set to auto_release=TRUE, the
  // openid_provider_form won't be called at all and this access check won't
  // be effective.
  elseif ($form_id == 'openid_provider_form') {
    global $user;
    if ($user->uid == 0) {
      drupal_goto('user/login');
    }
    elseif ($rp = openid_sso_provider_rps_load_by_realm($form_state['storage']['realm'])) {
      drupal_set_title('');
      // TODO: Make it configurable
      //global $user;
      //unset($form['submit_always']);
      //$form['submit_once']['#value'] = t('Log in');
      //$form['intro']['#value'] = '<div class="sso-message">' . t('Log in to <strong>@relying_party</strong> as @user?', array('@relying_party' => $rp->label, '@user' => $user->name)) . '</div>';
      //$form['other_user']['#value'] = l(t('Not @user?', array('@user' => $user->name)), 'sso/logout/redirect', array('query' => 'realm=' . urlencode($rp->realm)));
    }
    else {
      drupal_set_message(t('The OpenID relying party you are trying to login from is not registered for use.'));
      drupal_access_denied();
      exit();
    }
  }
  elseif (in_array($form_id, array('openid_redirect_form', 'openid_provider_form')) && (module_exists('securepages') && variable_get('securepages_enable', FALSE))) {
    $action_url = parse_url($form['#action']);
    // Change scheme to https for secure realm access
    $url = 'https://' . $action_url['host'] . $action_url['path'];
    $form['#action'] = $url;
  }

  // Add option for trusted sites from SSO relying party objects.
  elseif ($form_id == 'openid_provider_admin_settings') {
    $form['sitelist']['openid_provider_sites_auto_release']['#options']['whitelist'] = t('Whitelist an trusted SSO relying parties');
  }
}


/**
 * Implements hook_openid_provider_site_management_alter()
 */
function openid_sso_provider_openid_provider_site_management_alter(&$whitelist = array(), &$blacklist = array(), &$context = array()) {

  // Check if realm exists
  if (isset($context['realm'])) {

    // Load relying party object
    $rp = openid_sso_provider_rps_load_by_realm($context['realm']);

    // Add to whitelist when marked as trusted.
    if ($rp->trusted) {
      $whitelist[] = $context['realm'];
    }
  }
}

/**
 * Custom submit handler for user profile form. Add SSO redirect to form if
 * present.
 */
function openid_sso_provider_user_submit($form, &$form_state) {
  // openid_sso_provider_redirect has been set in openid_prover_sso_form_alter().
  if (isset($_SESSION['openid_sso_provider_redirect'])) {
    $form_state['redirect'] = $_SESSION['openid_sso_provider_redirect'];
    unset($_SESSION['openid_sso_provider_redirect']);
  }
}

/**
 * Implements hook_mail_alter().
 *
 * On user / login forms replace login uri and login url with urls that contain
 * the requesting RP's realm as destination.
 *
 * @see openid_sso_provider_form_alter().
 */
function openid_sso_provider_mail_alter(&$message) {

  // Check if coming from a user register or password changing form.
  if (in_array($message['id'], array('user_register_no_approval_required', 'user_register_pending_approval', 'user_password_reset'))) {

    // Check if the user is coming from an openid relying party.
    if (isset($_SESSION['openid_provider']['request'])) {

      // Check if the realm does really exist on the provider.
      if ($rp = openid_sso_provider_rps_load_by_realm($_SESSION['openid_provider']['request']['openid.realm'])) {

        // Prepare variables.
        $variables = array('user' => $message['params']['account']);
        $variables['relying_party'] = $rp;

        // Get the message's body text again.
        $message_body = _user_mail_text(substr($message['id'], 5) . '_body', $message['language'], $variables, FALSE);

        // Replace the message text this time with altered tokens.
        $message['body'][0] = token_replace($message_body, $variables, array('language' => $language, 'callback' => 'openid_sso_provider_mail_tokens', 'sanitize' => FALSE, 'clear' => TRUE));
      }
    }
  }
}


/**
 * Token callback to add unsafe tokens for user mails.
 *
 * This function is used by the token_replace() call at the end of
 * openid_sso_provider_mail_alter() to set up some additional tokens that can be
 * used in email messages generated by user_mail().
 *
 * @param $replacements
 *   An associative array variable containing mappings from token names to
 *   values (for use with strtr()).
 * @param $data
 *   An associative array of token replacement values. If the 'user' element
 *   exists, it must contain a user account object with the following
 *   properties:
 *   - login: The UNIX timestamp of the user's last login.
 *   - pass: The hashed account login password.
 * @param $options
 *   Unused parameter required by the token_replace() function.
 */
function openid_sso_provider_mail_tokens(&$replacements, $data, $options) {

  if (isset($data['user'])) {
    // Add relying party realm for later redirect.
    $replacements['[user:one-time-login-url]'] = user_pass_reset_url($data['user']) . '?realm=' . urlencode($data['relying_party']->realm);
    $replacements['[user:cancel-url]'] = user_cancel_url($data['user']);
  }
}


/**
 * Logout page callback. Logs out and initiates new authentication request if
 * a RP realm is present in $_GET.
 */
function openid_sso_provider_logout_page() {
  openid_sso_provider_logout();
  if ($rp = openid_sso_provider_rps_load_by_realm($_GET['realm'])) {
    drupal_goto($rp->realm . 'sso/init');
  }
  drupal_goto();
}

/**
 * Logout form. This form is being visited by an RP after logging out the user.
 * The user is presented with an option to log out on the OP (this site) as
 * well or stay logged in. In both cases the final destination is the RP where
 * the user departed.
 */
function openid_sso_provider_logout_form($form, &$form_state) {
  if ($rp = openid_sso_provider_rps_load_by_realm($_GET['realm'])) {
    if (user_is_logged_in()) {
      $form = array();
      $form_state['cache']['rp'] = $rp;
      $form['logout_redirect'] = array(
        '#type' => 'hidden',
        '#value' => $_GET['logout_redirect'],
      );
      $form['message'] = array(
        '#markup' => '<div class="sso-message">' . t('You logged out of <strong>@relying_party</strong>, would you also like to log out of <strong>@provider</strong>?', array('@relying_party' => $rp->label, '@provider' => variable_get('site_name', 'Drupal'))) . '</div>',
      );
      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Log out'),
        '#submit' => array('openid_sso_provider_logout_submit'),
      );
      $form['cancel'] = array(
        '#type' => 'submit',
        '#value' => t('Keep me logged in'),
        '#submit' => array('openid_sso_provider_logout_cancel'),
      );
      return $form;
    }
    if (isset($_GET['logout_redirect'])) {
      drupal_goto($_GET['logout_redirect']);
    }
    else {
      drupal_goto($rp->realm);
    }
  }
  return array();
}

/**
 * Submit handler for openid_sso_provider_logout_form().
 */
function openid_sso_provider_logout_submit($form, &$form_state) {
  openid_sso_provider_logout();

  // Redirect user to the RP where she came from.
  if (isset($form_state['values']['logout_redirect'])) {
    drupal_goto($form_state['values']['logout_redirect']);
  }
  elseif (isset($form_state['cache']['rp']->realm)) {
    drupal_goto($form_state['cache']['rp']->realm);
  }
}

/**
 * Submit handler for openid_sso_provider_logout_form().
 */
function openid_sso_provider_logout_cancel($form, &$form_state) {
  // Redirect user to the RP where she came from.
  if (!empty($form_state['values']['logout_redirect'])) {
    drupal_goto($form_state['values']['logout_redirect']);
  }
  elseif (isset($form_state['cache']['rp']->realm)) {
    drupal_goto($form_state['cache']['rp']->realm);
  }
}

/**
 * Log out - taken from user_logout(). Difference: no redirect at the end of the
 * function.
 *
 * @see user_logout()
 */
function openid_sso_provider_logout() {
  global $user;

  watchdog('user', 'Session closed for %name.', array('%name' => $user->name));

  module_invoke_all('user_logout', $user);

  // Destroy the current session, and reset $user to the anonymous user.
  session_destroy();
}

/**
 * Page callback for rendering a list of trusted sites.
 */
function openid_sso_provider_relying_parties_opml() {

  // Generate an OPML of trusted sites.
  $output = '<?xml version="1.0" encoding="utf-8"?>' . "\n";
  $output .= '<opml version="2.0">' . "\n";
  $output .= '<head>' . "\n";
  $output .= '  <title>' . t('Trusted relying parties for !site', array('!site' => variable_get('site_name', 'Drupal'))) . '</title>' . "\n";
  $output .= '  <dateCreated>' . format_date(REQUEST_TIME, 'custom', 'r', NULL) . '</dateCreated>' . "\n";
  $output .= '</head>' . "\n";
  $output .= '<body>' . "\n";
  foreach (openid_sso_provider_get_relying_parties() as $rp) {
    $output .= '  <outline text="' . check_plain($rp->label) . '" htmlUrl="' . check_url($rp->realm) . '"/>' . "\n";
  }
  $output .= '</body>' . "\n";
  $output .= '</opml>';

  drupal_add_http_header('Content-Type', 'text/x-opml');
  print $output;
}

/**
 * Page callback for a user specified by openid identity argument.
 * The full openid identifier must be specified in the query string under the
 * 'identifier' key.
 */
function openid_sso_provider_user_page() {
  if (isset($_GET['identifier'])) {
    if ($uid = openid_sso_provider_get_uid($_GET['identifier'])) {
      drupal_goto("user/{$uid}");
    }
  }
  drupal_not_found();
  exit;
}

/**
 * Retrieve the UID from an openid identifier URL on *this* OpenID provider.
 */
function openid_sso_provider_get_uid($identifier) {
  global $base_url;
  $path = str_replace("{$base_url}/", '', $identifier);
  $args = explode('/', $path);
  if ($args[0] === 'user' && is_numeric($args[1]) && $args[2] === 'identity') {
    return $args[1];
  }
  return FALSE;
}



/**
 * Retrieves a realm when set.
 *
 * @return
 *   Returns a realm url or false if not been set.
 */
function openid_sso_provider_get_realm() {

  // Initialize variable.
  $realm = NULL;

  // Try to discover realm.
  if (isset($_SESSION['openid_provider']['request']['openid.realm'])) {
    $realm = $_SESSION['openid_provider']['request']['openid.realm'];
    $_SESSION['openid_sso_realm'] = $_SESSION['openid_provider']['request']['openid.realm'];
  }
  elseif (isset($_GET['realm'])) {
    $realm = $_GET['realm'];
    $_SESSION['openid_sso_realm'] = $_GET['realm'];
  }
  elseif (isset($_POST['openid_realm'])) {
    $realm = $_POST['openid_realm'];
    $_SESSION['openid_sso_realm'] = $_POST['openid_realm'];
  }
  elseif (isset($_SESSION['openid_sso_realm'])) {
    $realm = $_SESSION['openid_sso_realm'];
  }

  // Check if realm is a has a valid url syntax.
  if (valid_url($realm)) {
    return $realm;
  }

  // Return FALSE in case no valid realm has been found.
  return FALSE;
}


function openid_sso_provider_get_origin() {

  $realm = openid_sso_provider_get_realm();
  $rp = openid_sso_provider_rps_load_by_realm($realm);
  return $rp;
}
