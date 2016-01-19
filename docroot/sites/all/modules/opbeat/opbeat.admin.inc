<?php
/**
 * @file
 * Administration interface for Opbeat module.
 */

/**
 * Admin settings form.
 */
function opbeat_admin_settings_form($form, &$form_state) {

  $form['opbeat_api_endpoint'] = array(
    '#type' => 'textfield',
    '#title' => t('Opbeat API Endpoint'),
    '#description' => t('Base URL of Opbeat API calls, should normally be %default', array('%default' => OPBEAT_DEFAULT_API_ENDPOINT)),
    '#default_value' => variable_get('opbeat_api_endpoint', OPBEAT_DEFAULT_API_ENDPOINT),
    '#required' => TRUE,
  );

  $form['authentication'] = array(
    '#type' => 'fieldset',
    '#title' => t('Authentication'),
    '#description' => t('Opbeat authentication settings. Can be found in Opbeat’s web interface, on the settings page for the “app” you want this Drupal site registered with in Opbeat.'),
  );

  $form['authentication']['opbeat_application_id'] = array(
    '#type' => 'textfield',
    '#title' => t('App ID'),
    '#default_value' => variable_get('opbeat_application_id', ''),
    '#required' => TRUE,
  );

  $form['authentication']['opbeat_organisation_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Organisation ID'),
    '#default_value' => variable_get('opbeat_organisation_id', ''),
    '#required' => TRUE,
  );

  $form['authentication']['opbeat_secret_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret token'),
    '#default_value' => variable_get('opbeat_secret_token', ''),
    '#required' => TRUE,
  );

  $form['opbeat_severity_levels'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Severity levels'),
    '#description' => t('Select the watchdog severity levels to send to Opbeat. Default is to send errors and everything above.'),
    '#options' => _opbeat_admin_severity_levels(),
    '#default_value' => variable_get('opbeat_severity_levels', array(WATCHDOG_EMERGENCY, WATCHDOG_ALERT, WATCHDOG_CRITICAL, WATCHDOG_ERROR)),
    '#multiple' => TRUE,
  );

  return system_settings_form($form);
}

/**
 * Provides an array of Drupal's severity levels, for the settings form.
 */
function _opbeat_admin_severity_levels() {
  return array(
    WATCHDOG_EMERGENCY => t('Emergency'),
    WATCHDOG_ALERT => t('Alert'),
    WATCHDOG_CRITICAL => t('Critical'),
    WATCHDOG_ERROR => t('Error'),
    WATCHDOG_WARNING => t('Warning'),
    WATCHDOG_NOTICE => t('Notice'),
    WATCHDOG_INFO => t('Info'),
    WATCHDOG_DEBUG => t('Debug'),
  );
}
