<?php

/**
 * @file
 * User page callbacks for the janrain_capture module.
 */

/**
 * Callback for the janrain_capture/oauth menu item. This serves as the
 * redirect_uri Capture redirects the user to and performs the authentication.
 */
function janrain_capture_oauth() {
  global $user;

  $query = drupal_get_query_parameters($_GET, array('q', 'code'));

  // Get the front page URL to the ajax function in the signin screen
  $front_page_url = url('<front>', array('absolute' => TRUE));

  // I'm not sure about this origin parameter. It doesn't seem to be used by
  // Drupal or its modules. I think what was meant is 'destination', but let's
  // leave it here for backwards compatibility, in case some site relies on it.
  $origin = isset($query['origin']) ? $query['origin'] : FALSE;
  if (!$origin) {
    $destination = isset($query['destination']) ? $query['destination'] : FALSE;
  }

  $url_type = isset($query['url_type']) ? $query['url_type'] : FALSE;

  $ver = variable_get('janrain_capture_ver', JANRAIN_CAPTURE_VERSION_DEFAULT);

  if ($ver == JANRAIN_CAPTURE_VERSION_LEGACY) {
    $janrain_capture_fields   = variable_get('janrain_capture_fields', array());
    $janrain_capture_main     = variable_get('janrain_capture_main', array());
    $janrain_capture_optional = variable_get('janrain_capture_optional', array());
  } else {
    $janrain_capture_fields   = variable_get('janrain_capture_fields2', array());
    $janrain_capture_main     = variable_get('janrain_capture_main2', array());
    $janrain_capture_main     = array_merge($janrain_capture_main, variable_get('janrain_capture_ui2', array()));
    $janrain_capture_optional = variable_get('janrain_capture_federate2', array());
    $janrain_capture_optional = array_merge($janrain_capture_optional, variable_get('janrain_capture_backplane2', array()));

    if (isset($query['verification_code'])) {
      $url_type='verify';
    }

    // Verify or forgot password
    if ($url_type) {
      module_load_include('inc', 'janrain_capture', 'includes/janrain_capture.widget');

      $widget_js = janrain_capture_widget_js();
      $screen = _janrain_capture_get_screen("{$url_type}.html");
      $js = _janrain_capture_get_screen("{$url_type}.js");

      $output = <<<AUTH
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <script type="text/javascript">
{$js}
{$widget_js}
    </script>
  </head>
  <body>
{$screen}
  </body>
</html>
AUTH;

      echo $output;
      return NULL;
    }
  }

  if (empty($_GET['code'])) {
    $no_oauth = module_invoke_all('janrain_capture_no_oauth');

    if (empty($no_oauth) || !in_array(FALSE, $no_oauth)) {
      drupal_set_message(t('No Oauth token found!'), 'error');

      echo $front_page_url;
      drupal_exit();
    }
  }
  else {
    $redirect_uri_options = array(
      'absolute' => TRUE,
      'query' => $query,
    );

    $settings = variable_get('janrain_capture_main2', array());

    if (!empty($settings['mobile_friendly'])) {
      $proto_redirect_uri = $_SESSION['janrain_capture_redirect_uri'];
    }
    else {
      $proto_redirect_uri = 'janrain_capture/oauth';
    }
    $redirect_uri = url($proto_redirect_uri, $redirect_uri_options);

    $api = new JanrainCaptureApi();

    $profile = $api->newAccessToken($_GET['code'], $redirect_uri) ? $api->loadUserEntity() : FALSE;

    if (!$profile || $profile['stat'] != 'ok') {
      drupal_set_message(t('We were unable to complete your request.'), 'error');
      watchdog('janrain_capture', 'Failed to obtain a Capture record', array(), WATCHDOG_ERROR);
    }
    else {
      $store_email = isset($janrain_capture_fields['capture_no_email']) ? $janrain_capture_fields['capture_no_email'] : FALSE;

      if ($store_email) {
        $_SESSION['janrain_capture_email'] = $profile['result']['email'];
      }

      // Check to see if a Drupal user with this email and UUID exists
      $account = user_load_by_mail($profile['result']['email']);

      if ($account) {
        $uuid = entity_metadata_wrapper('user', $account)->field_janrain_capture_uuid->value();

        if (!$uuid) {
          $is_admin = user_access('administer site configuration', $account);

          if ($is_admin) {
            $_SESSION['janrain_capture_admin_signin'] = TRUE;

            // Redirect to the admin signin verification endpoint
            echo url('janrain_capture/admin_signin', array('absolute' => TRUE));

            drupal_exit();
          }
        }
      }
      else {
        $account = new stdClass();
        $account->name = $profile['result']['uuid'];
        $account->mail = $store_email ?
          $profile['result']['email'] :
          $profile['result']['uuid'] . '@localhost';
        $account->status = 1;
      }

      // Add Capture-related information to the local account
      module_load_include('inc', 'janrain_capture', 'includes/janrain_capture.signin');
      _janrain_capture_setup_local_account($account, $profile);

      $action = isset($_SESSION['janrain_capture_action']) ? $_SESSION['janrain_capture_action'] : NULL;

      if (!_janrain_capture_user_verified($profile, $janrain_capture_fields['capture_enforce_verification'])) {
        if ($action == 'finish_third_party' || $action == 'legacy_register') {
          if (isset($_SESSION['janrain_capture_email'])){
            drupal_set_message(t('A verification link has been sent to @email. Please check your email.', array('@email' => $_SESSION['janrain_capture_email'])), 'status');
          }
          else {
            drupal_set_message(t('A verification link has been sent. Please check your email.'), 'status');
          }
        } else {
          $args = array(
            'action' => 'resend_verification_email',
            'access_token' => $_SESSION['janrain_capture_access_token'],
            'redirect_uri' => url('janrain_capture/resend_verification_email', array('absolute' => TRUE)),
          );

          $resend_link = janrain_capture_url($args);

          $email_unverified = module_invoke_all('janrain_capture_email_unverified', $resend_link);

          if (empty($email_unverified) || !in_array(FALSE, $email_unverified)) {
            drupal_set_message(t('Your email address has not yet been verified. Please check your email and try again. <a href="@resend-link">Click here</a> to have this email resent.', array('@resend-link' => $resend_link)), 'error');
          }
        }
      }
      else {
        $form_state['uid'] = $account->uid;

        user_login_submit(array(), $form_state);

        // Re-sync Capture data to make sure we're up to date.
        janrain_capture_sync_account($account, $profile['result']);

        $account = user_save($account);

        module_invoke_all('janrain_capture_user_authenticated', $profile['result'], $account, FALSE);
      }
    }
  }

  if ($ver == JANRAIN_CAPTURE_VERSION_LEGACY) {
    $signin_redirect = url('janrain_capture/signin_redirect', array('absolute' => TRUE));

    $output = <<<OAUTH_END
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
  <body>
    <p>Please wait...</p>
    <script type="text/javascript">
      var redirect_url = null;
      var regex = /[\#\?]destination\=([^\&]*)/;
      var match = regex.exec(window.parent.location.href);
      if (match && match.length == 2) {
        redirect_url = "{$signin_redirect}/?destination=" + decodeURIComponent(match[1]);
      }
      if (window.location.href != window.parent.location.href) {
        if (window.parent.location.href.indexOf("logout") > 1) {
          window.parent.location.href = "{$front_page_url}";
        } else {
          if (redirect_url) {
            window.parent.location.href = redirect_url;
          } else {
            window.parent.location.reload();
          }
        }
      } else {
        window.location.href = redirect_url || "{$front_page_url}";
      }
    </script>
  </body>
</html>
OAUTH_END;

    echo $output;
    return NULL;
  }

  echo $front_page_url;
  drupal_exit();
}

/**
 * Helper function to see if the user is verified
 */
function _janrain_capture_user_verified($profile, $enforce_verification) {
  if ($enforce_verification && $profile['result']['emailVerified'] == NULL) {
    return FALSE;
  } else {
    return TRUE;
  }
}

/**
 * Callback to show a Drupal login block to verify that a user is an admin
 */
function janrain_capture_admin_signin() {
  global $user;

  if (user_is_logged_in()) {
    $api = new JanrainCaptureApi();
    $profile = $api->loadUserEntity();

    if (in_array('administrator', $user->roles) && $profile['result']['email'] == $user->mail) {
      // Add Capture data to the local account
      janrain_capture_sync_account($user, $profile['result']);
      $user = user_save($user);
      module_invoke_all('janrain_capture_user_authenticated', $profile['result'], $user, FALSE);

      drupal_get_messages(NULL, TRUE);
      drupal_set_message(t('Success! Your Drupal account has been associated with Capture.'), 'status');

      drupal_goto();
    } else {
      $tmp = NULL;
      session_destroy();
      user_module_invoke('logout', $tmp, $user);
      $user = drupal_anonymous_user();

      drupal_set_message(t('The user account must match the user that was logged in using Capture. Please sign into Capture again.'), 'error');
      drupal_goto();
    }
  } else {
    drupal_set_message(t('A Drupal administrator account already exists with this email address. Please sign in using your Drupal username and password to associate this account with Capture.'), 'warning');

    $form = drupal_get_form('user_login_block');

    return $form;
  }
}

/**
 * Callback to redirect the user to the destination they were looking for before
 * signing in via Capture.
 */
function janrain_capture_signin_redirect() {
  drupal_goto();
}

/**
 * Callback for the janrain_capture/profile menu item. Ensures the access_token
 * is valid before redirecting the user to the Capture profile screen.
 */
function janrain_capture_profile() {
  $method = isset($_GET['method']) ? $_GET['method'] : '';
  $callback = isset($_GET['callback']) ? $_GET['callback'] : 'Drupal.janrainCapture.closeProfileEditor';
  $janrain_capture_main = variable_get('janrain_capture_main', array());
  $janrain_capture_optional = variable_get('janrain_capture_optional', array());
  $redirect_uri = url('janrain_capture/oauth', array('absolute' => TRUE));

  if (REQUEST_TIME >= $_SESSION['janrain_capture_expires_in']) {
    $api = new JanrainCaptureApi();
    $api->refreshAccessToken();
  }

  $args = array(
    'action' => 'profile' . $method,
    'access_token' => $_SESSION['janrain_capture_access_token'],
    'callback' => $callback,
  );

  $url = janrain_capture_url($args);

  drupal_goto($url);
}

/**
 * Callback for the janrain_capture/profile_sync menu item. Retrieves
 * the most recent data from Capture and stores values locally.
 */
function janrain_capture_profile_sync() {
  global $user;

  $api = new JanrainCaptureApi();
  $profile = $api->loadUserEntity();

  if (!$profile) {
    drupal_set_message(t('We were unable to complete your request.'), 'error');
    watchdog('janrain_capture', 'Failed to obtain a Capture record', array(), WATCHDOG_ERROR);
    return;
  }

  // Make sure the account object is fully populated.
  $account = user_load($user->uid);
  janrain_capture_sync_account($account, $profile['result']);
  if ($account = user_save($account)) {
    $profile_updated_hook = module_invoke_all('janrain_capture_user_profile_updated', $profile['result'], $account);
    if (empty($profile_updated_hook) || !in_array(FALSE, $profile_updated_hook)) {
      drupal_goto();
    }
  }
  else {
    drupal_set_message(t('We were unable to complete your request.'), 'error');
    watchdog('janrain_capture', 'Failed to save Capture data to user', array(), WATCHDOG_ERROR);
  }
}

/**
 * Callback for the janrain_capture/resend_verification_email menu item.
 * Displays a confirmation message that a verification email was resent.
 */
function janrain_capture_resend_verification_email() {
  $hook = module_invoke_all('janrain_capture_verification_resent');
  if (empty($hook) || !in_array(FALSE, $hook)) {
    if (isset($_SESSION['janrain_capture_email'])) {
      drupal_set_message(t('A verification link has been sent to @email. Please check your email.', array('@email' => $_SESSION['janrain_capture_email'])), 'status');
    }
    else {
      drupal_set_message(t('A verification link has been sent. Please check your email.'), 'status');
    }
    drupal_goto();
  }
}

/**
 * Callback the janrain_capture/token_expired menu item. Logs out a user
 * due to an expired session.
 */
function janrain_capture_token_expired() {
  global $user;
  $tmp = NULL;
  session_destroy();
  user_module_invoke('logout', $tmp, $user);
  $user = drupal_anonymous_user();

  drupal_set_message(t('Your session has expired. Please sign in again.'), 'error');
  drupal_goto();
}

/**
 * Callback for the janrain_capture/simple_logout menu item.
 *
 * Logs out the user without redirecting to a regular Drupal page. This endpoint
 * can be used by services that need to log the user out without incurring the
 * overhead of a redirect and processing of a full-blown Drupal page. It is
 * particularly useful for Janrain SSO's Javascript, which calls the logout link
 * using a dynamically created iframe and may break if iframe busters are used
 * by the Drupal site.
 */
function janrain_capture_simple_logout_page() {
  global $user;

  watchdog('janrain_capture', 'Session closed for %name.', array('%name' => $user->name));
  module_invoke_all('user_logout', $user);
  // Destroy the current session, and reset $user to the anonymous user.
  session_destroy();

  $output = <<<EOF
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
  <body>
    <p>You have been logged out.</p>
  </body>
</html>
EOF;

  print $output;
  return NULL;
}
