<?php
/**
 * @file
 * Settings forms for Commerce Estonian Payments.
 */

/**
 * SEB Settings form.
 */
function seb_settings_form($settings = NULL) {
  $form = array();
  $settings = (array) $settings + array(
    'banklink_url' => '',
    'merchant_id' => '',
    'merchant_private_key' => '',
    'merchant_private_key_passphrase' => '',
    'merchant_public_key' => '',
  );
  $form['banklink_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Banklink url'),
    '#default_value' => $settings['banklink_url'],
  );
  $form['merchant_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant id'),
    '#default_value' => $settings['merchant_id'],
  );
  $form['merchant_private_key'] = array(
    '#type' => 'textarea',
    '#title' => t('SEB merchant private key'),
    '#default_value' => $settings['merchant_private_key'],
  );
  $form['merchant_private_key_passphrase'] = array(
    '#type' => 'textfield',
    '#title' => t('SEB merchant private key passphrase'),
    '#default_value' => $settings['merchant_private_key_passphrase'],
  );
  $form['merchant_public_key'] = array(
    '#type' => 'textarea',
    '#title' => t('SEB merchant public key'),
    '#default_value' => $settings['merchant_public_key'],
  );
  return $form;
}

/**
 * Swedbank Settings form.
 */
function swedbank_settings_form($settings = NULL) {
  $form = array();
  $settings = (array) $settings + array(
    'banklink_url' => '',
    'merchant_id' => '',
    'merchant_private_key' => '',
    'merchant_private_key_passphrase' => '',
    'merchant_public_key' => '',
  );
  $form['banklink_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Banklink url'),
    '#default_value' => $settings['banklink_url'],
  );
  $form['merchant_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant id'),
    '#default_value' => $settings['merchant_id'],
  );
  $form['merchant_private_key'] = array(
    '#type' => 'textarea',
    '#title' => t('Swedbank merchant private key'),
    '#default_value' => $settings['merchant_private_key'],
  );
  $form['merchant_private_key_passphrase'] = array(
    '#type' => 'textfield',
    '#title' => t('Swedbank merchant private key passphrase'),
    '#default_value' => $settings['merchant_private_key_passphrase'],
  );
  $form['merchant_public_key'] = array(
    '#type' => 'textarea',
    '#title' => t('Swedbank merchant public key'),
    '#default_value' => $settings['merchant_public_key'],
  );
  return $form;
}

/**
 * Sampo Settings form.
 */
function sampo_settings_form($settings = NULL) {
  $form = array();
  $settings = (array) $settings + array(
    'banklink_url' => '',
    'merchant_id' => '',
    'merchant_private_key' => '',
    'merchant_private_key_passphrase' => '',
    'merchant_public_key' => '',
  );
  $form['banklink_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Banklink url'),
    '#default_value' => $settings['banklink_url'],
  );
  $form['merchant_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant id'),
    '#default_value' => $settings['merchant_id'],
  );
  $form['merchant_private_key'] = array(
    '#type' => 'textarea',
    '#title' => t('Sampo merchant private key'),
    '#default_value' => $settings['merchant_private_key'],
  );
  $form['merchant_private_key_passphrase'] = array(
    '#type' => 'textfield',
    '#title' => t('Sampo merchant private key passphrase'),
    '#default_value' => $settings['merchant_private_key_passphrase'],
  );
  $form['merchant_public_key'] = array(
    '#type' => 'textarea',
    '#title' => t('Sampo merchant public key'),
    '#default_value' => $settings['merchant_public_key'],
  );
  return $form;
}

/**
 * Nordea Settings form.
 */
function nordea_settings_form($settings = NULL) {
  $form = array();
  $settings = (array) $settings + array(
    'banklink_url' => '',
    'merchant_id' => '',
    'merchant_mac' => '',
    'hash' => 'md5',
  );
  $form['banklink_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Banklink url'),
    '#default_value' => $settings['banklink_url'],
  );
  $form['merchant_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant id'),
    '#default_value' => $settings['merchant_id'],
  );
  $form['merchant_mac'] = array(
    '#type' => 'textfield',
    '#title' => t('Nordea merchant MAC key'),
    '#default_value' => $settings['merchant_mac'],
  );
  $form['hash'] = array(
    '#type' => 'textfield',
    '#title' => t('Nordea hash'),
    '#default_value' => $settings['hash'],
  );
  return $form;
}