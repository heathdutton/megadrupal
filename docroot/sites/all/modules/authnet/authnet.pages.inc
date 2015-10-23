<?php

/**
 * @file
 * Authorize.net API Pages
 */

/**
 * Settings form.
 */
function authnet_settings_form($form, &$form_state) {

  // General API settings
  $form['api'] = array(
    '#type' => 'fieldset',
    '#title' => t('API Login ID and Transaction Key'),
    '#description' => t('The Authorize.net Login ID and Transaction Key is provided to you when you sign up for an account with Authorize.net. This information is required to interact with Authorize.net.  It is different from your login ID and password and may be found through your account settings page.'),
  );
  $form['api']['authnet_login_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Login ID'),
    '#default_value' => variable_get('authnet_login_id', ''),
    '#required' => TRUE,
  );
  $form['api']['authnet_transaction_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Transaction Key'),
    '#default_value' => variable_get('authnet_transaction_key', ''),
    '#required' => TRUE,
  );
  $form['api']['authnet_sandbox'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sandbox mode'),
    '#description' => t('If sandbox mode is turned on, transactions will be posted to the Authorize.Net test server. Otherwise, they will be posted directly to the live Authorize.Net gateway.'),
    '#default_value' => variable_get('authnet_sandbox', FALSE),
  );
  $form['api']['authnet_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debug mode'),
    '#description' => t('Log full API response messages from Authorize.net to watchdog for debugging.'),
    '#default_value' => variable_get('authnet_debug', FALSE),
  );

  // CIM settings
  $form['cim_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('CIM settings'),
    '#description' => t('These settings pertain to the Authorize.Net Customer Information Management service.'),
  );
  $form['cim_settings']['authnet_cim_validation_mode'] = array(
    '#type' => 'radios',
    '#title' => t('CIM validation mode'),
    '#description' => t('Validation mode allows you to generate a test transaction at the time you create a customer profile. In Test Mode, only field validation is performed. In Live Mode, a transaction is generated and submitted to the processor with the amount of &#36;0.00 or &#36;0.01. If successful, the transaction is immediately voided.'),
    '#options' => array(
      'none' => t('Off'),
      'testMode' => t('Test Mode'),
      'liveMode' => t('Live Mode'),
    ),
    '#default_value' => variable_get('authnet_cim_validation_mode', 'none'),
  );

  return system_settings_form($form);
}
