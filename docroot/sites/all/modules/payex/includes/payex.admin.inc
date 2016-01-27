<?php

/**
 * @file
 * Administration pages for PayEx module.
 */

/**
 * Administration settings form.
 */
function payex_admin_settings_form($form, $form_state) {
  $defaults = variable_get('payex_settings', array('vat' => 0, 'merchant_account' => '', 'currency' => 'DKK', 'live' => 0));

  $form['live'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable live transactions'),
    '#description' => t('Warning, only use this for a live site running real transactions.'),
    '#default_value' => $defaults['live'],
  );

  $form['merchant_account'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant account'),
    '#description' => t('PayEx merchant account number. Can be found under “Merchant Profile” in the PayEx admin panel.'),
    '#default_value' => $defaults['merchant_account'],
    '#required' => TRUE,
  );

  // When the encryption key is not set, provide a simple field for it.
  if (!isset($defaults['encryption_key'])) {
    $form['encryption_key'] = array(
      '#type' => 'textfield',
      '#title' => t('Encryption key'),
      '#description' => t('PayEx encryption key. You can generate a new encryption under “Merchant Profile” in the PayEx admin panel. If you do, remember to change it here as well, or all payments will fail.'),
      '#required' => TRUE,
    );
  }
  else {
    $form['encryption_wrapper'] = array(
      '#type' => 'fieldset',
      '#title' => t('Encryption key'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form['encryption_wrapper']['encryption_key'] = array(
      '#type' => 'textfield',
      '#title' => t('New encryption key'),
      '#description' => t('If your encryption key has changed, you can change the one stored in the Drupal database here.'),
    );
  }

  $form['currency'] = array(
    '#type' => 'select',
    '#title' => t('Default currency'),
    '#description' => t('Select the default currency for PayEx payments. This setting may be overridden by implemeting modules.'),
    '#required' => TRUE,
    '#options' => array(
      'DKK' => t('Danish krone'),
      'EUR' => t('Euro'),
      'GBP' => t('Pound sterling'),
      'NOK' => t('Norwegian krone'),
      'SEK' => t('Swedish krona'),
      'USD' => t('U.S. Dollar'),
    ),
    '#default_value' => $defaults['currency'],
  );

  $form['vat'] = array(
    '#type' => 'textfield',
    '#title' => t('Default VAT rate'),
    '#description' => t('Select the default VAT (value added tax) rate for PayEx payments. 0 for no VAT. This setting may be overridden by implemeting modules.'),
    '#required' => TRUE,
    '#default_value' => $defaults['vat'],
    '#size' => 2,
    '#field_suffix' => '%',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save settings',
  );

  return $form;
}

/**
 * Validation for administration settings.
 */
function payex_admin_settings_form_validate(&$form, &$form_state) {
  if ((integer) trim($form_state['values']['merchant_account']) < 2) {
    form_set_error('merchant_account', t('Merchant account must be a number larger than 1'));
  }

  $vat = (integer) trim($form_state['values']['vat']);
  if ($vat < 0 || $vat > 100) {
    form_set_error('vat', t('VAT rate must be a number between 0 and 100.'));
  }
}

/**
 * Submit handler for administration settings.
 */
function payex_admin_settings_form_submit(&$form, &$form_state) {
  $defaults = variable_get('payex_settings', array());

  // New settings to be stored.
  $new_settings = array(
    'currency' => $form_state['values']['currency'],
    'merchant_account' => (integer) trim($form_state['values']['merchant_account']),
    'vat' => (integer) trim($form_state['values']['vat']),
    'live' => $form_state['values']['live'],
  );

  // If a new encryption key was filled in, store it.
  $key = trim($form_state['values']['encryption_key']);
  if (!empty($key)) {
    $new_settings['encryption_key'] = $key;
  }
  // Otherwise, keep the old value.
  else {
    $new_settings['encryption_key'] = $defaults['encryption_key'];
  }

  // Save our new settings.
  variable_set('payex_settings', $new_settings);
}

