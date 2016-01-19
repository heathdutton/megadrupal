<?php
// @file
/**
 * Callback for admin configuration
 * Get/Set the variable spamagement_log_successful_registrations
 *
 */
function spamagement_admin() {
  $form = array();

  $form['spamagement_log_successful_registrations'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log name, mail, and IP address for all successful user registrations.'),
    '#description' => t('Log the name, mail, and IP address used during registration for all successful user registration attempts. This can be helpful in identifying the source of spam registrations that get past other filters that may be enabled on your site.'),
    '#default_value' => variable_get('spamagement_log_successful_registrations', NULL),
  );

  return system_settings_form($form);
}
