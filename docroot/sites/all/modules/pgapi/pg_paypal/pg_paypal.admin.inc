<?php

/*
 * @file
 * pg_paypal.admin.inc
 * Admin interface for paypal pgapi module
 */

/**
 * Paypal settings form
 */
function pg_paypal_settings($form, &$form_state) {
  $form['pg_paypal_action_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Action url'),
    '#default_value' => variable_get('pg_paypal_action_url', cPaypalActionUrl),
    '#description' => t("Please enter action url. Default: !url", array('!url' => cPaypalActionUrl)),
  );
  $form['pg_paypal_validation_url'] = array(
    '#type' => 'textfield',
    '#title' => t('PayPal IPN "request back" validation URL'),
    '#default_value' => variable_get('pg_paypal_validation_url', cPaypalValidationUrl),
    '#description' => t("URL where IPN 'request back' is posted to validate the transaction.\nDefault: !url", array('!url' => cPaypalValidationUrl)),
  );
  $form['pg_paypal_email'] = array(
    '#type' => 'textfield',
    '#title' => t('PayPal Receiver Email'),
    '#default_value' => variable_get('pg_paypal_email', ''),
    '#description' => t("Primary email address of the payment recipent. This is also your main paypal email address."),
  );
  $currency_codes = array(
    'USD' => 'USD - U.S. Dollars',
    'EUR' => 'EUR - Euros',
    'AUD' => 'AUD - Australian Dollars',
    'CAD' => 'CAD - Canadian Dollars',
    'GBP' => 'GBP - Pounds Sterling',
    'JPY' => 'JPY - Yen',
    'NZD' => 'NZD - New Zealand Dollar',
    'CHF' => 'CHF - Swiss Franc',
    'HKD' => 'HKD - Hong Kong Dollar',
    'SGD' => 'SGD - Singapore Dollar',
    'SEK' => 'SEK - Swedish Krona',
    'DKK' => 'DDK - Danish Krone',
    'PLN' => 'PLN - Polish Zloty',
    'NOK' => 'NOK - Norwegian Krone',
    'HUF' => 'HUF - Hungarian Forint',
    'CZK' => 'CZK - Czech Koruna',
  );
  $form['pg_paypal_currency_code'] = array(
    '#type' => 'select',
    '#title' => t('Currency code'),
    '#options' => $currency_codes,
    '#default_value' => variable_get('pg_paypal_currency_code', 'USD'),
    '#description' => t("The currecy code that PayPal should process the payment in."),
  );
  $form['pg_paypal_rate'] = array(
    '#type' => 'textfield',
    '#title' => t('Rate to site currency'),
    '#default_value' => variable_get('pg_paypal_rate', '1.00'),
    '#description' => t("Please enter Paypal rate according to site currency."),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit paypal settings'),
  );
  return system_settings_form($form);
}

/**
 * Paypal settings form validator
 */
function pg_paypal_settings_validate($form, &$form_state) {
  if ($form_state['values']['pg_paypal_rate'] <= 0) {
    form_set_error('pg_paypal_rate', t('%rate must be more than 0.', array('%rate' => $form_state['values']['pg_paypal_rate'])));
  }
}
