<?php

/**
 * @file
 * Administrative page callbacks and forms for Commerce Card on File.
 */


/**
 * Builds the card on file settings form.
 */
function commerce_cardonfile_settings_form($form, &$form_state) {
  $form['commerce_cardonfile_storage'] = array(
    '#type' => 'radios',
    '#title' => t('Card on file storage method'),
    '#description' => t('The storage method governs the checkbox that appears beneath the credit card form during checkout.'),
    '#options' => array(
      'opt-in' => t('Show a checkbox letting customers opt-in to storing their card on file.'),
      'opt-out' => t('Show a checkbox letting customers opt-out of storing their card on file.'),
      'required' => t('Do not show a checkbox and always store their card on file.'),
    ),
    '#default_value' => variable_get('commerce_cardonfile_storage', 'opt-in'),
  );

  $form['commerce_cardonfile_selector'] = array(
    '#type' => 'radios',
    '#title' => t('Card on file form element'),
    '#description' => t('The specified form element will be used during checkout when a customer has previously stored a card on file.'),
    '#options' => array(
      'radios' => t('Radio buttons (e.g. Visa belonging to John Doe: Ends in 1111, Expires 05/2015)'),
      'select' => t('Select list (e.g. Visa ending in 1111, Exp. 05/2015)'),
    ),
    '#default_value' => variable_get('commerce_cardonfile_selector', 'radios'),
  );

  $form['commerce_cardonfile_multiple'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow customers to store more than one card on file if it is supported by your payment gateway.'),
    '#default_value' => variable_get('commerce_cardonfile_multiple', TRUE),
  );

  $form['commerce_cardonfile_hide_cc_radio_button'] = array(
    '#type' => 'checkbox',
    '#title' => t('If credit card is the only available payment method, hide its radio button if the customer has a card on file.'),
    '#default_value' => variable_get('commerce_cardonfile_hide_cc_radio_button', TRUE),
  );

  return system_settings_form($form);
}
