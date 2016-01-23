<?php

/**
 * @file
 * Admin settings.
 */

function ms_ep_admin_form($method) {
  $form = array();
  $form['main'] = array(
    '#type' => 'fieldset',
    '#title' => t('Main Settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['main'][$method . '_banklink_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Banklink url'),
    '#ms_override' => TRUE,
    '#default_value' => variable_get($method . '_banklink_url', ''),
  );
  $form['main'][$method . '_merchant_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant id'),
    '#ms_override' => TRUE,
    '#default_value' => variable_get($method . '_merchant_id', ''),
  );
  $form['main'][$method . '_merchant_private_key'] = array(
    '#type' => 'textarea',
    '#title' => t('Merchant private key'),
    '#ms_override' => TRUE,
    '#default_value' => variable_get($method . '_merchant_private_key', ''),
  );
  $form['main'][$method . '_merchant_private_key_passphrase'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant private key passphrase'),
    '#ms_override' => TRUE,
    '#default_value' => variable_get($method . '_merchant_private_key_passphrase', ''),
  );
  $form['main'][$method . '_merchant_public_key'] = array(
    '#type' => 'textarea',
    '#title' => t('Merchant public key'),
    '#ms_override' => TRUE,
    '#default_value' => variable_get($method . '_merchant_public_key', ''),
  );

  return $form;
}

/**
 * SEB Settings form.
 */
function ms_ep_seb_admin() {
  return ms_ep_admin_form('ms_ep_seb');
}

/**
 * Swedbank Settings form.
 */
function ms_ep_swedbank_admin() {
  return ms_ep_admin_form('ms_ep_swedbank');
}

/**
 * Sampo Settings form.
 */
function ms_ep_sampo_admin() {
  return ms_ep_admin_form('ms_ep_sampo');
}

/**
 * Nordea Settings form.
 */
function ms_ep_nordea_admin() {
  $method = 'ms_ep_nordea';
  $form = array();
  $form['main'] = array(
    '#type' => 'fieldset',
    '#title' => t('Main Settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['main'][$method . '_banklink_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Banklink url'),
    '#ms_override' => TRUE,
    '#default_value' => variable_get($method . '_banklink_url', ''),
  );
  $form['main'][$method . '_merchant_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant id'),
    '#ms_override' => TRUE,
    '#default_value' => variable_get($method . '_merchant_id', ''),
  );
  $form['main'][$method . '_merchant_mac'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant MAC key'),
    '#ms_override' => TRUE,
    '#default_value' => variable_get($method . '_merchant_mac', ''),
  );
  $form['main'][$method . '_hash'] = array(
    '#type' => 'select',
    '#title' => t('Hash'),
    '#options' => array(
      'md5' => 'MD5',
      'sha1' => 'SHA1',
    ),
    '#ms_override' => TRUE,
    '#default_value' => variable_get($method . '_hash', ''),
  );

  return $form;
}