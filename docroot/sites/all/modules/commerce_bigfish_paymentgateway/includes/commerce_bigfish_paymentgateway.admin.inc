<?php

/**
 * @file
 * Implements administration form for Commerce Payment Gateway. 
 */

/**
 * Implements hook_form(). 
 */
function commerce_bigfish_paymentgateway_configure_form($form, $form_state) {
  _commerce_bigfish_paymentgateway_load_library();

  $form['commerce_bigfish_paymentgateway_store_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Store name'),
    '#description' => t('The webshop store name what included to contract.'),
    '#required' => TRUE,
    '#default_value' => variable_get('commerce_bigfish_paymentgateway_store_name', \BigFish\PaymentGateway::SDK_TEST_STORE_NAME),
  );

  $form['commerce_bigfish_paymentgateway_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API key'),
    '#description' => t('The API key from BIG FISH Payment Gateway service.'),
    '#required' => TRUE,
    '#default_value' => variable_get('commerce_bigfish_paymentgateway_api_key', \BigFish\PaymentGateway::SDK_TEST_API_KEY),
  );

  $form['commerce_bigfish_paymentgateway_test_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Running test mode'),
    '#description' => t('Use test mode under developing webshop.'),
    '#return_value' => TRUE,
    '#default_value' => variable_get('commerce_bigfish_paymentgateway_test_mode', TRUE),
  );

  $form['commerce_bigfish_paymentgateway_encryption_key'] = array(
    '#type' => 'textarea',
    '#title' => t('Public encryption key'),
    '#description' => t('Public key used for encrypting sensitive data. (optional)'),
    '#default_value' => variable_get('commerce_bigfish_paymentgateway_encryption_key', \BigFish\PaymentGateway::SDK_TEST_ENCRYPT_PUBLIC_KEY),
  );

  return system_settings_form($form);
}
