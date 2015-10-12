<?php

/**
 * @file
 * Administrative page callbacks and forms for Commerce Bank Account on File.
 */


/**
 * Builds the bank account on file settings form.
 */
function commerce_bank_account_onfile_settings_form($form, &$form_state) {
  $form['commerce_bank_account_onfile_storage'] = array(
    '#type' => 'radios',
    '#title' => t('Bank account on file storage method'),
    '#description' => t('The storage method governs the checkbox that appears beneath the bank account form during checkout.'),
    '#options' => array(
      'opt-in' => t('Show a checkbox letting customers opt-in to storing their bank account on file.'),
      'opt-out' => t('Show a checkbox letting customers opt-out of storing their bank account on file.'),
      'required' => t('Do not show a checkbox and always store their bank account on file.'),
    ),
    '#default_value' => variable_get('commerce_bank_account_onfile_storage', 'opt-in'),
  );

  $form['commerce_bank_account_onfile_selector'] = array(
    '#type' => 'radios',
    '#title' => t('Bank account on file form element'),
    '#description' => t('The specified form element will be used during checkout when a customer has previously stored a bank account on file. Display label for each item would be like "John Doe (Type: Checking, Routing: 1234, Account: 9876, Bank: Example Bank)"'),
    '#options' => array(
      'radios' => t('Radio buttons'),
      'select' => t('Select list'),
    ),
    '#default_value' => variable_get('commerce_bank_account_onfile_selector', 'radios'),
  );

  // TODO: Not used anywhere. Use it.
  // $form['commerce_cardonfile_multiple'] = array(
  //   '#type' => 'checkbox',
  //   '#title' => t('Allow customers to store more than one card on file if it is supported by your payment gateway.'),
  //   '#default_value' => variable_get('commerce_cardonfile_multiple', TRUE),
  // );

  // $form['commerce_cardonfile_hide_cc_radio_button'] = array(
  //   '#type' => 'checkbox',
  //   '#title' => t('If credit card is the only available payment method, hide its radio button if the customer has a card on file.'),
  //   '#default_value' => variable_get('commerce_cardonfile_hide_cc_radio_button', TRUE),
  // );

  return system_settings_form($form);
}
