<?php
/**
 * @file
 * Common functions used by the Commerce NMI Direct Post modules.
 */

/**
 * Payment method callback: settings form.
 */
function commerce_nmi_direct_post_settings_form($settings = NULL) {
  $settings += array(
    'username' => 'demo',
    'password' => NULL,
    'processor' => NULL,
  );

  $form['username'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant account username'),
    '#default_value' => $settings['username'],
    '#required' => TRUE,
    '#description' => t('Username for the merchant account that transactions will be processed for. Use "demo" for testing.'),
  );

  $form['password'] = array(
    '#type' => 'password',
    '#title' => t('Merchant account password'),
    '#default_value' => $settings['password'],
    '#required' => empty($settings['password']),
    '#description' => t('Password for the merchant account that transactions will be processed for. Use "password" for testing.'),
  );

  $form['processor'] = array(
    '#type' => 'textfield',
    '#title' => t('Processor id'),
    '#default_value' => $settings['processor'],
    '#description' => t('If the merchant account has multiple MIDs then specify the processor id to route transactions to.'),
  );

  return $form;
}

/**
 * Payment method callback: settings form submit.
 */
function commerce_nmi_direct_post_settings_form_submit(&$form, &$form_state) {
  $current_password = $submitted_password = NULL;
  if (!empty($form_state['input']['parameter']['payment_method']['settings']['payment_method']['settings']['password'])) {
    $submitted_password = $form_state['input']['parameter']['payment_method']['settings']['payment_method']['settings']['password'];
  }

  if (!empty($form_state['element_settings']['payment_method']['settings']['password'])) {
    $current_password = $form_state['element_settings']['payment_method']['settings']['password'];
  }

  // If a new password was saved then encrypt it and set it into the form input
  // and the rules action. Otherwise, set them to the existing password to avoid
  // clearing it out.
  if (!empty($submitted_password)) {
    $encrypted_password = commerce_nmi_encrypt_password($submitted_password);
    $form_state['input']['parameter']['payment_method']['settings']['payment_method']['settings']['password'] = $encrypted_password;
    $form_state['rules_element']->settings['payment_method']['settings']['password'] = $encrypted_password;
  }
  else {
    $form_state['input']['parameter']['payment_method']['settings']['payment_method']['settings']['password'] = $current_password;
    $form_state['rules_element']->settings['payment_method']['settings']['password'] = $current_password;
  }
}
