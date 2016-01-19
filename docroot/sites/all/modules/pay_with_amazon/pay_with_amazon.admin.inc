<?php

/**
 * @file
 * Administrative page callbacks for the pay_with_amazon module.
 */

/**
 * Form for Pay With Amazon Settings.
 */
function pay_with_amazon_admin_settings_form($form_state) {
  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
  );

  $form['account']['pay_with_amazon_merchantid'] = array(
    '#title' => t('Merchant ID'),
    '#type' => 'textfield',
    '#default_value' => variable_get('pay_with_amazon_merchantid', ''),
    '#description' => t('You can find this value from !link', array('!link' => 'https://sellercentral.amazon.in/gp/cba/seller/account/settings/user-settings-view.html')),
    '#size' => 50,
    '#maxlength' => 50,
    '#required' => TRUE,
  );
  $form['account']['pay_with_amazon_access_key_id'] = array(
    '#title' => t('Access Key ID'),
    '#type' => 'textfield',
    '#default_value' => variable_get('pay_with_amazon_access_key_id', ''),
    '#size' => 50,
    '#maxlength' => 50,
    '#required' => TRUE,
  );
  $form['account']['pay_with_amazon_secret_key_id'] = array(
    '#title' => t('Secret Key ID'),
    '#type' => 'textfield',
    '#default_value' => variable_get('pay_with_amazon_secret_key_id', ''),
    '#size' => 50,
    '#maxlength' => 50,
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
