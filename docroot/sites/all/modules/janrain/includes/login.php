<?php
/**
 * @file
 * All login related functions.
 */

/**
 * Implements hook_form_user_register_form_alter().
 * @todo-3.1 display icon and displayname in addition to identifier
 */
function janrain_form_user_register_form_alter(&$form, &$form_state) {
  // Janrain info in session show user the identifier that will be linked.
  $identifiers = DrupalAdapter::getSessionItem('identifiers');
  $form['janrain_display'] = array(
    '#type' => 'item',
    '#title' => t('Your Social Identifier'),
    '#description' => t('This Social Identifier will be attached to your account after registration.'),
    '#markup' => '<p>' . check_plain($identifiers[0]) . '</p>',
  );
  $form['janrain_identifiers'] = array(
    '#type' => 'value',
    '#value' => DrupalAdapter::getSessionItem('identifiers'),
  );

  // Prepopulate any data we have.
  $form['account']['mail']['#default_value'] = DrupalAdapter::getSessionItem('email');
  $form['account']['name']['#default_value'] = DrupalAdapter::getSessionItem('name');

  // Add submit handler.
  $form['#submit'][] = 'janrain_form_user_register_form_submit';
}

/**
 * Submit handler for register form.
 */
function janrain_form_user_register_form_submit(&$form, &$form_state) {
  $account = $form['#user'];

  _janrain_link_identifiers($account);
  _janrain_clear_session();

  // Log user in.
  $form_state['uid'] = $account->uid;
  user_login_submit(array(), $form_state);
}

/**
 * Login validation handler.
 */
function janrain_login_validate(&$form, &$form_state) {
  $identifiers = _janrain_get_identifiers();

  if (empty($identifiers)) {
    watchdog('janrain', '{{user}} attempting to login without Janrain.', array('{{user}}' => $form_state['values']['name']), WATCHDOG_INFO);
    return;
  }

  // Deserialize the Janrain data from the session.
  $profiledata = json_decode(DrupalAdapter::getSessionItem('profile'), TRUE);
  $profile = new \janrain\Profile($profiledata);

  // Try to find user by social identifier.
  foreach ($identifiers as $external_id) {
    $account = user_external_load($external_id);
    if ($account) {
      // Found user by identifier.
      _janrain_clear_session();
      $form_state['uid'] = $account->uid;
      watchdog('janrain', '{{user}} logged in by identifier.',
        array('{{user}}' => check_plain(format_username($account))), WATCHDOG_INFO);
      return;
    }
  }

  // User not found by external id try to link on secondary credentials.
  $vemail = $profile->getFirst('$.profile.verifiedEmail');
  $vemail = valid_email_address($vemail) ? $vemail : FALSE;
  $email = $profile->getFirst('$.profile.email');
  $email = valid_email_address($email) ? $email : FALSE;
  $strict_email = variable_get('user_email_verification', TRUE);

  // No users with that identifier, find by verifiedEmail.
  $account = $vemail ? user_load_by_mail($vemail) : FALSE;
  if ($vemail && $account) {
    _janrain_link_identifiers($account);
    _janrain_clear_session();
    $form_state['uid'] = $account->uid;
    watchdog('janrain', '{{user}} logged in by verified email.',
      array('{{user}}' => check_plain(format_username($account))), WATCHDOG_INFO);
    return;
  }

  // No users by identifier or verifiedEmail, try by email.
  $account = $email ? user_load_by_mail($email) : FALSE;
  if ($email && $account) {
    // Honor Drupal email validation.  Admin accounts require verified email.
    if (!$strict_email && ($account->uid != 1)) {
      // Drupal doesn't care about email verification.
      _janrain_link_identifiers($account);
      _janrain_clear_session();
      $form_state['uid'] = $account->uid;
      watchdog('janrain', '{{user}} logged in by unverified email.',
        array('{{user}}' => check_plain(format_username($account))), WATCHDOG_INFO);
      return;
    }
    else {
      // Drupal cares about email verification.
      drupal_set_message(t('User discovered via unverified email, please enter password to prove account ownership and link accounts.'), 'warning');
    }
  }

  // Not found by linked identifier, email, or verifiedEmail do new user.
  $regform = array();
  $regform['values']['name'] = $profile->getFirst('$.profile.preferredUsername');
  $regform['values']['mail'] = $vemail ?: $email;
  $regform['values']['pass']['pass1'] = user_password();
  $regform['values']['pass']['pass2'] = $regform['values']['pass']['pass1'];
  $regform['values']['op'] = t('Create new acount');
  $regform['values']['janrain_identifiers'] = DrupalAdapter::getSessionItem('identifiers');
  drupal_form_submit('user_register_form', $regform);
  if (form_get_errors()) {
    // couldn't user social login, show message, go to registration.
    $message = t('Unable to log you in with @id', array('@id' => filter_xss($identifiers[0])));
    drupal_set_message($message, 'warning', FALSE);
    // Leaves this stack, session is intact.
    drupal_goto('user/register');
  }

  // No errors with the new registration, load by the email provided and login.
  $account = user_load_by_mail($regform['values']['mail']);
  $form_state['uid'] = $account->uid;
  watchdog('janrain', '{{user}} registered.',
    array('{{user}}' => check_plain(format_username($account))), WATCHDOG_INFO);

  // Go ahead and login.
  user_login_submit(array(), $form_state);
}
