<?php

/**
 * @file
 * Commerce_smartpay.admin.inc.
 *
 * Provides a single admin page to contain all Smartpay settings.
 */

/**
 * Settings form containing all configuration for the Smartpay module.
 */
function commerce_smartpay_settings_form($form, &$form_state) {

  $form['account_details'] = array(
    '#type' => 'fieldset',
    '#title' => t('Your Account'),
    '#collapsible' => TRUE,
  );
  $form['account_details'][COMMERCE_SMARTPAY_MERCHANT_ACCOUNT] = array(
    '#type' => 'textfield',
    '#title' => t('Smartpay Merchant Account'),
    '#description' => t('The merchant account we want to process this payment with.'),
    '#required' => TRUE,
    '#default_value' => variable_get(COMMERCE_SMARTPAY_MERCHANT_ACCOUNT, ''),
  );
  $form['account_details'][COMMERCE_SMARTPAY_SKIN_CODE] = array(
    '#type' => 'textfield',
    '#title' => t('Smartpay Skin code'),
    '#description' => t('The skin to be used.'),
    '#required' => FALSE,
    '#default_value' => variable_get(COMMERCE_SMARTPAY_SKIN_CODE, ''),
  );
  $form['shared_secret'] = array(
    '#type' => 'fieldset',
    '#title' => t('Shared secret key'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['shared_secret'][COMMERCE_SMARTPAY_SHARED_SECRET_TEST] = array(
    '#type' => 'textfield',
    '#title' => t('Test shared secret'),
    '#description' => t('The shared HMAC key for TEST environment.'),
    '#required' => FALSE,
    '#default_value' => variable_get(COMMERCE_SMARTPAY_SHARED_SECRET_TEST, ''),
  );
  $form['shared_secret'][COMMERCE_SMARTPAY_SHARED_SECRET_LIVE] = array(
    '#type' => 'textfield',
    '#title' => t('Live shared secret'),
    '#description' => t('The shared HMAC key for LIVE environment.'),
    '#required' => FALSE,
    '#default_value' => variable_get(COMMERCE_SMARTPAY_SHARED_SECRET_LIVE, ''),
  );
  $form['Transaction'] = array(
    '#type' => 'fieldset',
    '#title' => t('Transaction settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['Transaction'][COMMERCE_SMARTPAY_TRANSACTION_MODE] = array(
    '#type' => 'radios',
    '#title' => t('Transaction mode'),
    '#description' => t('Adjust to live transactions when you are ready to start processing actual payments.'),
    '#options' => array(
      COMMERCE_SMARTPAY_MODE_LIVE => t('Live transactions in a live account.'),
      COMMERCE_SMARTPAY_MODE_TEST => t('Test transactions in a test account.'),
    ),
    '#default_value' => variable_get(COMMERCE_SMARTPAY_TRANSACTION_MODE, COMMERCE_SMARTPAY_MODE_TEST),
  );

  return system_settings_form($form);
}
