<?php
/**
 * @file
 * Administrative page and settngs form for librato module.
 */

/**
 * Implements system_settings_form().
 */
function librato_admin_settings_form() {
  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['account']['librato_config_email'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail'),
    '#default_value' => variable_get('librato_config_email', ''),
    '#required' => TRUE,
    '#description' => t('This email address is the one used when you created an account, if you dont have an account create one now: <a href="@librato" target="_blank">librato.com</a>.', array('@librato' => 'http://www.librato.com')),
  );

  $form['account']['librato_config_token'] = array(
    '#type' => 'textfield',
    '#title' => 'Token',
    '#default_value' => variable_get('librato_config_token', ''),
    '#required' => TRUE,
  );

  $form['default_metrics'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default metrics instruments'),
  );

  $form['default_metrics']['librato_config_report_php_errors'] = array(
    '#type' => 'checkbox',
    '#title' => t('Report php errors'),
    '#default_value' => variable_get('librato_config_report_php_errors', 1),
    '#description' => t('Reports the number of php errors at the time of the cron when activated.'),
  );

  return system_settings_form($form);
}
