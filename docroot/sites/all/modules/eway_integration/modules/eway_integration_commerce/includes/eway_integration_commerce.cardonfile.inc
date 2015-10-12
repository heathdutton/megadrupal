<?php

/**
 * @file
 * Commerce cardonfile integration.
 */

/**
 * Create card callback.
 * @return bool
 */
function eway_integration_commerce_cardonfile_create($form, $form_state, $payment_method, $card) {
  $configs = array(
    'auth' => array(
      'user' => $payment_method['settings']['api_username'],
      'pass' => $payment_method['settings']['api_password'],
    ),
    'method' => $payment_method['method_id'],
    'sandbox' => $payment_method['settings']['server'] == 'sandbox' ? TRUE : FALSE,
  );
  if (isset($form_state['values']['credit_card']['number'])) {
    $card->cardno = $form_state['values']['credit_card']['number'];
  }
  $token_request = eway_integration_commerce_create_token_user_request($card);
  $token_response = eway_integration_api_request($token_request, $configs);

  if ($token_response && isset($token_response->Customer->TokenCustomerID)) {
    $card->remote_id = $token_response->Customer->TokenCustomerID;
    $card->status = 1;
    return $card;
  }
  else {
    $message = t('Unable to save your card data due to a server error');
    if (isset($token_response->ErrorPhases)) {
      $message = $message . ' - ' . implode(",", $token_response->ErrorPhases);
    }
    drupal_set_message($message, 'error');
    return FALSE;
  }
}

/**
 * Delete card callback.
 * @return bool
 */
function eway_integration_commerce_cardonfile_charge() {
  return TRUE;
}

/**
 * Charge card callback.
 * @return bool
 */
function eway_integration_commerce_cardonfile_update() {
  return TRUE;
}

/**
 * Delete card callback.
 * @return bool
 */
function eway_integration_commerce_cardonfile_delete() {
  return TRUE;
}

/**
 * Alter commerce_cardonfile_checkout_pane_form.
 * @param $payment
 * @param $form
 */
function eway_integration_commerce_commerce_cardonfile_checkout_pane_form_alter($payment, $form) {
  // If cardonfile module loaded, reset fields in payment details.
  if (isset($form['commerce_payment']['payment_details']['cardonfile'])) {
    if (isset($form['commerce_payment']['payment_details']['cardonfile']['#value']) && $form['commerce_payment']['payment_details']['cardonfile']['#value'] == 'new') {
      return;
    }
    // Move set default checkbox under security code field.
    $temp_field = $form['commerce_payment']['payment_details']['cardonfile_instance_default'];
    unset($form['commerce_payment']['payment_details']['cardonfile_instance_default']);
    // Add a security code which is required by eWAY API.
    $form['commerce_payment']['payment_details']['code'] = array(
      '#type' => 'textfield',
      '#title' => t('CVN code'),
      '#default_value' => '',
      '#attributes' => array('autocomplete' => 'off'),
      '#maxlength' => 4,
      '#size' => 4,
      '#states' => array(
        'invisible' => array(
          ':input[name$="[payment_method]"]' => array('checked' => TRUE),
        ),
        'visible' => array(
          ':input[name$="[cardonfile]"]' => array('!value' => 'new'),
        ),
        'disabled' => array(
          ':input[name$="[cardonfile]"]' => array('value' => 'new'),
        ),
      ),
    );
    // eWAY integration - Client Side Encryption.
    $form['commerce_payment']['payment_details']['eway_encrypt_code'] = array(
      '#type' => 'hidden',
      '#default_value' => '',
    );
    $form['commerce_payment']['payment_details']['cardonfile_instance_default'] = $temp_field;
  }
}
