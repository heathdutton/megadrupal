<?php

/**
 * @file
 * Admin page callbacks for the system configuration settings.
 */

/**
 * Module admin configuraion form.
 */
function drupaltosugar_system_configuration() {
  drupal_add_js(drupal_get_path('module', 'drupaltosugar'), '/drupaltosugar.js');
  $form['fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('System Configuration'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['fieldset']['drupaltosugar_sugarcrm_url'] = array(
    '#type' => 'textfield',
    '#title' => t('SugarCRM URL'),
    '#description' => t('Enter the URL of SugarCRM System like http://example.com'),
    '#default_value' => variable_get('drupaltosugar_sugarcrm_url', NULL),
  );
  $form['fieldset']['drupaltosugar_sugarcrm_username'] = array(
    '#type' => 'textfield',
    '#title' => t('User Name'),
    '#description' => t('Enter your SugarCRM User Name'),
    '#default_value' => variable_get('drupaltosugar_sugarcrm_username', NULL),
  );
  $form['fieldset']['drupaltosugar_sugarcrm_pass'] = array(
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#description' => t('SugarCRM password'),
    '#default_value' => variable_get('drupaltosugar_sugarcrm_pass', NULL),
  );
  $form['fieldset']['test_connection'] = array(
    '#type' => 'button',
    '#value' => 'Test Connection',
    '#executes_submit_callback' => TRUE,
    '#submit' => array('drupaltosugar_test_connection'),
  );
  return system_settings_form($form);
}

/**
 * Function for testing sugarCRM connection.
 */
function drupaltosugar_test_connection(&$form, &$form_state) {
  // Setting system variables if not set already.
  $crm_url = trim($form_state['values']['drupaltosugar_sugarcrm_url']);
  $crm_user_name = trim($form_state['values']['drupaltosugar_sugarcrm_username']);
  $crm_pass = trim($form_state['values']['drupaltosugar_sugarcrm_pass']);
  variable_set('drupaltosugar_sugarcrm_url', $crm_url);
  variable_set('drupaltosugar_sugarcrm_username', $crm_user_name);
  variable_set('drupaltosugar_sugarcrm_pass', $crm_pass);
  // Generating session id.
  $session_id = drupaltosugar_get_connection();
  if (array_key_exists('id', $session_id)) {
    drupal_set_message(t('Successfully connected'));
  }
  else {
    drupal_set_message($session_id['error'], 'error');
  }
}

/**
 * Cron configuration form for mapped webforms.
 */
function drupaltosugar_cron_configuration_form() {
  $webform_data = drupaltosugar_webform_mapped_nid();
  $interval = array(
    '60' => '1 Minute',
    '120' => '2 Minute',
    '360' => '5 Minute',
    '900' => '15 Minute',
    '1800' => '30 Minute',
    '3600' => '1 Hour',
    '86400' => '1 Day',
    '604800' => '1 Week',
  );

  $form['cron_interval'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cron Interval'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#prefix' => '<div class = "webform-cron-interval">',
    '#suffix' => '</div>',
  );
  if (!count($webform_data)) {
    $form['cron_interval']['empty'] = array(
      '#markup' => t('<div>There are no webform mapped with SugarCRM</div>'),
    );
    return $form;
  }

  foreach ($webform_data as $key => $value) {
    $variable_name = 'webform_' . $key;
    $form['cron_interval']['interval'][$key] = array(
      '#type' => 'select',
      '#options' => $interval,
      '#default_value' => variable_get($variable_name, '60'),
      '#required' => TRUE,
      '#prefix' => '<div class = "webform-interval">' . '<lable>' . t('!form_name', array('!form_name' => $value)) . '</lable>',
      '#suffix' => '</div>',
    );
  }
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Function to save cron configuration for each mapped webform.
 */
function drupaltosugar_cron_configuration_form_submit($form, &$form_state) {
  foreach ($form_state['values'] as $nid => $interval) {
    $webform_name = 'webform_' . $nid;
    $webform_next_exe = 'webform_next_exe' . $nid;
    if (is_numeric($nid)) {
      variable_set($webform_name, $interval);
      variable_set($webform_next_exe, 0);
    }
  }
  drupal_set_message(t("Cron configuration has been saved"));
}
