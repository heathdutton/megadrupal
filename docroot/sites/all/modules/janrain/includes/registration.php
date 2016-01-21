<?php
/**
 * @file
 * Profile related functions.
 */

/**
 * Implements hook_form_user_login_form_submit().
 */
function janrain_form_user_login_form_submit(&$form, &$form_state) {
  $identifiers = _janrain_get_identifiers();
  if (!$identifiers) {
    return;
  }
  $account = user_load($form_state['uid']);
  _janrain_link_identifiers($account);
  _janrain_clear_session();
}

/**
 * Helper to validate login attempts using Janrain.
 */
function janrain_login_validate(&$form, &$form_state) {
  $identifiers = _janrain_get_identifiers();

  if (empty($identifiers)) {
    watchdog('janrain', '{{user}} attempting to login without Janrain.', array('{{user}}' => $form_state['values']['name']), WATCHDOG_WARNING);
    return;
  }

  // Try to login.
  // For capture this should succeed in a single iteration.
  foreach ($identifiers as $external_id) {
    $account = user_external_load($external_id);
    if ($account) {
      _janrain_link_identifiers($account);
      _janrain_clear_session();
      $form_state['uid'] = $account->uid;
      return;
    }
  }

  // Failed by all identifiers last ditch try email match.
  // @todo admin should detect this is required and unique in schema.
  $verified_email = DrupalAdapter::getSessionItem('verifiedEmail');
  $verified_email = valid_email_address($verified_email) ? $verified_email : FALSE;

  $account = $verified_email ? user_load_by_mail($verified_email) : FALSE;
  if ($verified_email && $account) {
    // Found user by trusted email.
    // Link them up, log them in, clear the session.
    watchdog('janrain', 'Found @name by verified email @email', array(
      '@name' => format_username($account),
      '@email' => filter_xss($verified_email)), WATCHDOG_DEBUG);
    _janrain_link_identifiers($account);
    _janrain_clear_session();
    $form_state['uid'] = $account->uid;
    return;
  }

  // No users found by identifiers or verified emails.

  $strict_email = variable_get('user_email_verification', TRUE);
  $email = DrupalAdapter::getSessionItem('email');
  if (!$strict_email) {
    // Drupal doesn't care if email address are verified #sosecure.
    $email = valid_email_address($email) ? $email : FALSE;
    // Lookup user by unverified email.
    $account = user_load_by_mail($email);
    // Try user login/link but skip admins when email unverified.
    if ($account && ($account->uid != 1)) {
      // Found user, link them up, log them in, clear the session.
      watchdog('janrain', 'Found @name by email @email', array(
        '@name' => format_username($account),
        '@email' => filter_xss($email)), WATCHDOG_DEBUG);
      _janrain_link_identifiers($account);
      _janrain_clear_session();
      $form_state['uid'] = $account->uid;
      return;
    }
  }

  // Did not find user by any identifiers, create a new account.
  // directly save user and then log them in according to drupal registration
  // status.
  // n.b. capture is authoritative.
  $account_info = array(
    'name' => DrupalAdapter::getSessionItem('name'),
    'init' => $identifiers[0],
    'mail' => $email,
    'access' => REQUEST_TIME,
    // Capture disables drupal based registration, so be sure to activate user.
    'status' => 1,
    'pass' => user_password(),
  );
  try {
    $new_user = user_save(drupal_anonymous_user(), $account_info);
    watchdog('janrain', 'Created user {{user}}', array(
      '{{user}}' => format_username($new_user)), WATCHDOG_NOTICE);
    _janrain_link_identifiers($new_user);
    $form_state['uid'] = $new_user->uid;
    _janrain_clear_session();
    user_login_submit(array(), $form_state);
  }
  catch (\Exception $e) {
    // User save failed.
    // Should only happen if required data is missing (shouldn't happen).
    // Or if there's a uniqueness violation on display name or email.
    drupal_set_message(t('An error occured with your login. Contact your Drupal site admin to resolve.'), 'error');
    watchdog_exception('janrain', $e, NULL, array(), WATCHDOG_EMERGENCY);
  }
}
