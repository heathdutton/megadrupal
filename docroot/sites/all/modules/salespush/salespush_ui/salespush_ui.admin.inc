<?php

/**
 * @file
 * Administrative user interface for SalesPush module.
 */

/**
 * Salespush configuration form.
 */
function salespush_ui_admin($form, &$form_state) {
  $form['salespush_login'] = array(
    '#type' => 'textfield',
    '#title' => t('Login for production salesforce.com'),
    '#default_value' => variable_get('salespush_login'),
    '#attributes' => array(
      'placeholder' => t('enter your production salesforce login'),
    ),
  );
  $form['salespush_pass'] = array(
    '#type' => 'password',
    '#title' => t('Password for production salesforce.com'),
    '#default_value' => variable_get('salespush_pass'),
    '#attributes' => array(
      'placeholder' => t('enter your production salesforce password'),
    ),
  );
  $form['salespush_token'] = array(
    '#type' => 'password',
    '#title' => t('Security token for production salesforce.com'),
    '#default_value' => variable_get('salespush_token'),
    '#attributes' => array(
      'placeholder' => t('enter your production salesforce security token'),
    ),
  );

  $form['salespush_prod_wsdl'] = array(
    '#type' => 'select',
    '#title' => t('WSDL file for production salesforce.com'),
    '#options' => salespush_get_wsdls(),
    '#default_value' => variable_get('salespush_prod_wsdl'),
    '#description' => t('Select wsdl file from salespush wsdl directory.'),
  );

  $form['salespush_sandbox_login'] = array(
    '#type' => 'textfield',
    '#title' => t('Login for sandbox test.salesforce.com'),
    '#default_value' => variable_get('salespush_sandbox_login'),
    '#attributes' => array(
      'placeholder' => t('enter your sanbox salesforce login'),
    ),
  );
  $form['salespush_sandbox_pass'] = array(
    '#type' => 'password',
    '#title' => t('Password for sandbox test.salesforce.com'),
    '#default_value' => variable_get('salespush_sandbox_pass'),
    '#attributes' => array(
      'placeholder' => t('enter your sandbox salesforce password'),
    ),
  );
  $form['salespush_sandbox_token'] = array(
    '#type' => 'password',
    '#title' => t('Security token for sandbox test.salesforce.com'),
    '#default_value' => variable_get('salespush_sandbox_token'),
    '#attributes' => array(
      'placeholder' => t('enter your salesforce sandbox security token'),
    ),
  );

  $form['salespush_sandbox_wsdl'] = array(
    '#type' => 'select',
    '#title' => t('WSDL file for sandbox test.salesforce.com'),
    '#options' => salespush_get_wsdls(),
    '#default_value' => variable_get('salespush_sandbox_wsdl'),
    '#description' => t('Select wsdl file from salespush wsdl directory.'),
  );

  return system_settings_form($form);
}

/**
 * SalesPush environment settings.
 */
function salespush_ui_environment($form, &$form_state) {
  $form['salespush_mapper_cron_environment'] = array(
    '#type' => 'select',
    '#title' => t('Environment for backend'),
    '#options' => array('sandbox' => 'sandbox', 'production' => 'production'),
    '#default_value' => variable_get('salespush_mapper_cron_environment', 'sandbox'),
    '#description' => t('Select proper environment to work with via cron jobs.'),
  );

  $form['salespush_mapper_cron_state'] = array(
    '#type' => 'checkbox',
    '#title' => t('State of salespush cron'),
    '#default_value' => variable_get('salespush_mapper_cron_state', FALSE),
    '#description' => t('Enable or disable pushing data via cron.'),
  );

  return system_settings_form($form);
}

/**
 * Get list of available wsdls in salespush/wsdl directory.
 *
 * @return array
 *   Array for #options element.
 */
function salespush_get_wsdls() {
  // Get list of files from /wsdl dir in salespush module root.
  $wsdl_path = drupal_get_path('module', 'salespush') . '/wsdl';
  $wsdl_dir_content = array_diff(scandir($wsdl_path), array('..', '.'));
  // Create array('filename' => 'filename') for #options data.
  $options = array_combine($wsdl_dir_content, $wsdl_dir_content);

  return $options;
}
