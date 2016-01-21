<?php
/**
 * @file
 * This is for the administrative settings for this module.
 */

/**
 * Email Verify administration settings form.
 *
 * @return array
 *   The admin settings form.
 */
function email_verify_admin_settings($form, &$form_state) {
  $form['email_verify_active'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable the Email Verify to verify email adresses'),
    '#default_value' => variable_get('email_verify_active', 0),
    '#description' => t('When enabled, Email Verify will check email addresses for validity.'),
  );

  $form['email_verify_email'] = array(
    '#type' => 'fieldset',
    '#title' => t('Forms to check'),
    '#collapsible' => TRUE,
    '#description' => t("Check the boxes for the forms you want to have this module check email addresses on."),
  );
  $form['email_verify_email']['email_verify_user_registration'] = array(
    '#type' => 'checkbox',
    '#title' => t("User registration"),
    '#default_value' => variable_get('email_verify_user_registration', 1),
  );
  $form['email_verify_email']['email_verify_user_profile'] = array(
    '#type' => 'checkbox',
    '#title' => t("User profile"),
    '#default_value' => variable_get('email_verify_user_profile', 1),
  );

  if (module_exists('contact')) {
    $form['email_verify_email']['email_verify_site_contact'] = array(
      '#type' => 'checkbox',
      '#title' => t("Site-wide contact"),
      '#default_value' => variable_get('email_verify_site_contact', 0),
    );
    $form['email_verify_email']['email_verify_personal_contact'] = array(
      '#type' => 'checkbox',
      '#title' => t("Personal contact"),
      '#default_value' => variable_get('email_verify_personal_contact', 0),
    );
  }

  $form['#submit'] = array('email_verify_admin_settings_submit');

  return system_settings_form($form);
}

/**
 * Form submit function.
 */
function email_verify_admin_settings_submit($form, &$form_state) {
  // Enable/disable mail sending subsystem.
  if ($form_state['values']['email_verify_active']) {
    if (!email_verify_activated()) {
      email_verify_enable_module();
    }
  }
}

/**
 * Checks the system for the capability to use this module.
 *
 * @todo
 *   This function works, but it needs some thought and potential rework, now
 *   that it is not in email_verify.install.
 */
function email_verify_enable_module() {
  // Check that fsockopen() works on port 25.
  // See: http://drupal.org/node/147883

  // What follows is an adapted version of email_verify_check().
  // The documentation http://api.drupal.org/api/5/function/hook_install says:
  //   "Note that since this function is called from a full bootstrap, all
  //    functions (including those in modules enabled by the current page
  //    request) are available when this hook is called. Use cases could be
  //    displaying a user message, or calling a module function necessary for
  //    initial setup, etc."
  // However, this does not seem to be the case, so we can't reuse
  // email_verify_check().

  // If a previous enable found port 25 closed or fsockopen() disabled, don't
  // test it again. Testing can cause a long delay on module enable. Completely
  // uninstall and then re-install this module to re-test.
  If (variable_get('email_verify_skip_mailbox', FALSE)) {
    return;
  }

  // Check if fsockopen() is disabled.
  if (!function_exists('fsockopen')) {
    $message = t('Email Verify will test email domains but not mailboxes because the fsockopen() function has been disabled.');
    variable_set('email_verify_skip_mailbox', TRUE);
    drupal_set_message($message, 'warning');
    return;
  }

  $host = 'drupal.org';
  // What SMTP servers should we contact?
  $mx_hosts = array();

  module_load_include('inc', 'email_verify', 'windows_compat');

  if (!getmxrr($host, $mx_hosts)) {
    // When there is no MX record, the host itself should be used.
    $mx_hosts[] = $host;
  }

  // Try to connect to one SMTP server.
  foreach ($mx_hosts as $smtp) {
    $connect = @fsockopen($smtp, 25, $errno, $errstr, 15);
    if (!$connect) {
      continue;
    }

    if (preg_match("/^220/", $out = fgets($connect, 1024))) {
      // OK, we have a SMTP connection.
      break;
    }
  }

  if (!$connect) {
    variable_set('email_verify_skip_mailbox', TRUE);
    $message = t('Email Verify will test email domains but not mailboxes because port 25 is closed on your host\'s firewall for security.');
    watchdog('email_verify', $message, array(), WATCHDOG_WARNING);
    drupal_set_message($message, 'warning');
  }
}
