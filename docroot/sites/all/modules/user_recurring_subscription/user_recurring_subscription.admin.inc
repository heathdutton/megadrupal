<?php

/**
 * @file
 *
 * Page callbacks for the user_recurring_subscription module.
 *
 * @author: ratanphp
 */

 /**
  * Credit card paypal gateway setting form.
  */
function user_recurring_subscription_payment_settings_form($form, &$form_state) {
  $form['base'] = array(
    '#type' => 'fieldset',
    '#title' => t('Credit Card (PayFlow Pro)'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => -10,
    '#group' => 'additional_settings',
  );

  $form['base']['user_recurring_subscription_server'] = array(
    '#type' => 'select',
    '#title' => t('API server'),
    '#description' => t('Sign up for and use a Sandbox account for testing.'),
    '#options' => array(
      'https://pilot-payflowpro.paypal.com' => t('Sandbox'),
      'https://payflowpro.paypal.com' => t('Live'),
    ),
    '#default_value' => variable_get('user_recurring_subscription_server', 'https://pilot-payflowpro.paypal.com'),
  );

  $form['base']['user_recurring_subscription_currency'] = array(
    '#type' => 'select',
    '#title' => t('Currency code'),
    '#description' => t('Transactions can only be processed in one of the listed currencies.'),
    '#options' => user_recurring_subscription_currency_array(),
    '#default_value' => variable_get('user_recurring_subscription_currency', 'USD'),
  );

  $form['base']['api'] = array(
    '#type' => 'fieldset',
    '#title' => t('API credentials'),
    '#collapsible' => TRUE,
    '#collapsed' => variable_get('user_recurring_subscription_user', '') != '',
  );

  $form['base']['api']['user_recurring_subscription_vendor'] = array(
    '#type' => 'textfield',
    '#title' => t('Vendor'),
    '#default_value' => variable_get('user_recurring_subscription_vendor', ''),
    '#required' => TRUE,
  );

  $form['base']['api']['user_recurring_subscription_partner'] = array(
    '#type' => 'textfield',
    '#title' => t('Partner'),
    '#default_value' => variable_get('user_recurring_subscription_partner', 'PayPal'),
    '#required' => TRUE,
  );

  $form['base']['api']['user_recurring_subscription_user'] = array(
    '#type' => 'textfield',
    '#title' => t('User'),
    '#default_value' => variable_get('user_recurring_subscription_user', ''),
    '#required' => TRUE,
  );

  $form['base']['api']['user_recurring_subscription_pass'] = array(
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#default_value' => variable_get('user_recurring_subscription_pass', ''),
    '#required' => TRUE,
  );
  return system_settings_form($form);
}
