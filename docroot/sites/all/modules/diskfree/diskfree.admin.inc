<?php

/**
 * @file
 * Disfree admin settings.
 */

/**
 * Settings form to configure utility paths and alerts.
 */
function diskfree_admin_settings() {
  $form = array();
  $form['diskfree_alert_threshold'] = array(
    '#type' => 'textfield',
    '#title' => t('Alert threshold percent'),
    '#description' => t('An integer between 0 and 100 which is the minimum disk space to trigger an alert of low disk space. A value of 100 effectively disables alerts.'),
    '#default_value' => variable_get('diskfree_alert_threshold', '95'),
  );
  $form['diskfree_alert_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Alert email address'),
    '#default_value' => variable_get('diskfree_alert_email', variable_get('site_mail', ini_get('sendmail_from'))),
  );
  $form['diskfree_alert_email_freq'] = array(
    '#type' => 'select',
    '#title' => t('Alert email sending frequency'),
    '#description' => t('How often to send email alerts per partition.'),
    '#options' => array(
      '300' => '5 minutes',
      '600' => '10 minutes',
      '900' => '15 minutes',
      '1800' => '30 minutes',
      '3600' => '1 hour',
      '7200' => '2 hours',
      '14400' => '4 hours',
      '28800' => '8 hours',
      '86400' => '1 day',
    ),
    '#default_value' => variable_get('diskfree_alert_email_freq', '3600'),
  );
  return system_settings_form($form);
}

/**
 * Implements hook_validate().
 */
function diskfree_admin_settings_validate($form, &$form_state) {
  $threshold = $form_state['values']['diskfree_alert_threshold'];
  if (!is_numeric($threshold) || $threshold < 0 || $threshold > 100) {
    form_set_error('diskfree_alert_threshold', t('Alert threshold must be an integer between 0 and 100, inclusive.'));
  }

  $mail = $form_state['values']['diskfree_alert_email'];
  if ($mail != '' && !valid_email_address($mail)) {
    form_set_error('diskfree_alert_email', t('The e-mail address %mail is not valid.', array('%mail' => $mail)));
  }
}
