<?php

/**
 * @file
 * Implements administration form for Commerce Payment Gateway. 
 */

/**
 * Implements hook_form(). 
 */
function commerce_bigfish_paymentgateway_configure_form($form, $form_state) {

  $form['commerce_bigfish_paymentgateway_store_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Store name'),
    '#description' => t('The name what included to contract.'),
    '#required' => TRUE,
    '#default_value' => variable_get('commerce_bigfish_paymentgateway_store_name', 'sdk_test'),
  );

  $form['commerce_bigfish_paymentgateway_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API key'),
    '#description' => t('The api key from Payment Gateway service.'),
    '#required' => TRUE,
    '#default_value' => variable_get('commerce_bigfish_paymentgateway_api_key', '86af3-80e4f-f8228-9498f-910ad'),
  );

  $form['commerce_bigfish_paymentgateway_test_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Running test mode'),
    '#description' => t('Use test mode under developing webshop.'),
    '#return_value' => TRUE,
    '#default_value' => variable_get('commerce_bigfish_paymentgateway_test_mode', TRUE),
  );

  return system_settings_form($form);
}
