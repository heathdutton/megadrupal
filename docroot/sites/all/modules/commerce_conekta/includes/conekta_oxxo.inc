<?php

/**
 * Returns the default settings for the PayPal WPP payment method.
 */
function commerce_conekta_oxxo_default_settings() {
  $default_currency = commerce_default_currency();

  return array(
    'api_username' => '',
    'api_password' => '',
    'api_signature' => '',
    'server' => 'sandbox',
    'code' => TRUE,
    'card_types' => drupal_map_assoc(array('visa', 'mastercard', 'amex', 'discover')),
    'currency_code' => in_array($default_currency, array_keys(commerce_paypal_currencies('paypal_wpp'))) ? $default_currency : 'USD',
    'allow_supported_currencies' => FALSE,
    'txn_type' => COMMERCE_CREDIT_AUTH_CAPTURE,
    'log' => array('request' => 0, 'response' => 0),
  );
}
