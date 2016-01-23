<?php
/**
 * @file
 * The login page.
 */

/**
 * FormAPI callback to build the login form.
 *
 * Ultimately this is a wrapper for user_login().
 */
function alt_login_page_login_form($form, &$form_state) {
  global $user;

  // If the user is already logged in, redirect to the homepage.
  if ($user->uid) {
    drupal_goto();
  }

  // Build the form.
  else {
    // Load the normal user login form.
    $form = user_login($form, $form_state);

    // Move the username validation to the front of the list, then add the
    // custom validation right after it.
    array_unshift($form['#validate'], 'user_login_name_validate');
    $form['#validate'][1] = 'alt_login_page_login_form_validate';

    foreach ($form['#validate'] as $index => $function) {
      if ($function == 'user_login_final_validate') {
        $form['#validate'][$index] = 'alt_login_final_validate';
      }
    }
    // Trigger the regular user login form submission callback.
    $form['#submit'][] = 'user_login_submit';

    // Redirect on login.
    $form_state['redirect'] = '<front>';

    // All done;
    return $form;
  }
}

/**
 * FAPI validation callback.
 *
 * Verifies that the requested user account has permission to use the
 * alternative login page.
 */
function alt_login_page_login_form_validate($form, &$form_state) {
  if (!empty($form_state['values']['name'])) {
    // Load the requested user.
    $account = user_load_by_name($form_state['values']['name']);

    // The user account does not have permission to use the alternative login
    // page.
    if (!empty($account) && !user_access('use alt login page', $account)) {
      form_set_error('name', t('This account does not have permission to login through this page and must use the <a href="!url">regular site login page</a>.', array('!url' => url('user/login'))));
    }
  }
}

/**
 * FormAPI callback to build the password reset form.
 *
 * Use the existing functionality, but redirect back to the alternate path.
 */
function alt_login_page_pass_form($form, &$form_state) {
  module_load_include('pages.inc', 'user');
  $form = user_pass();

  $form['#validate'][] = 'user_pass_validate';
  $form['#submit'][] = 'user_pass_submit';
  $form['#submit'][] = 'alt_pass_submit';

  return $form;
}

/**
 * FormAPI submission callback for alt_login_page_pass_form().
 */
function alt_pass_submit($form, &$form_state) {
  $form_state['redirect'] = variable_get('alt_login_page_path');
}

/**
 * The overridden final validation handler on the login form.
 *
 * Sets a form error if user has not been authenticated, or if too many
 * logins have been attempted. This validation function should always
 * be the last one.
 */
function alt_login_final_validate($form, &$form_state) {
  $path = variable_get('alt_login_page_path');
  if (empty($form_state['uid'])) {
    // Always register an IP-based failed login event.
    flood_register_event('failed_login_attempt_ip', variable_get('user_failed_login_ip_window', 3600));
    // Register a per-user failed login event.
    if (isset($form_state['flood_control_user_identifier'])) {
      flood_register_event('failed_login_attempt_user', variable_get('user_failed_login_user_window', 21600), $form_state['flood_control_user_identifier']);
    }

    if (isset($form_state['flood_control_triggered'])) {
      if ($form_state['flood_control_triggered'] == 'user') {
        form_set_error('name', format_plural(variable_get('user_failed_login_user_limit', 5), 'Sorry, there has been more than one failed login attempt for this account. It is temporarily blocked. Try again later or <a href="@url">request a new password</a>.', 'Sorry, there have been more than @count failed login attempts for this account. It is temporarily blocked. Try again later or <a href="@url">request a new password</a>.', array('@url' => url($path . '/password'))));
      }
      else {
        // We did not find a uid, so the limit is IP-based.
        form_set_error('name', t('Sorry, too many failed login attempts from your IP address. This IP address is temporarily blocked. Try again later or <a href="@url">request a new password</a>.', array('@url' => url($path . '/password'))));
      }
    }
    else {
      form_set_error('name', t('Sorry, unrecognized username or password. <a href="@password">Have you forgotten your password?</a>', array('@password' => url($path . '/password', array('query' => array('name' => $form_state['values']['name']))))));
      watchdog('user', 'Login attempt failed for %user.', array('%user' => $form_state['values']['name']));
    }
  }
  elseif (isset($form_state['flood_control_user_identifier'])) {
    // Clear past failures for this user so as not to block a user who might
    // log in and out more than once in an hour.
    flood_clear_event('failed_login_attempt_user', $form_state['flood_control_user_identifier']);
  }
}
