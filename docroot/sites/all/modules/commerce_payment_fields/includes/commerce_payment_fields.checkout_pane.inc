<?php

/**
 * @file
 * Callback functions for the Payment module's checkout panes.
 */

// Requires the default commerce payment pane functions.
module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.checkout_pane');

/**
 * Checkout pane callback: returns the payment pane's settings form.
 */
function commerce_payment_fields_commerce_payment_pane_settings_form($checkout_pane) {
  return commerce_payment_pane_settings_form($checkout_pane);
}

/**
 * Payment pane: form callback.
 */
function commerce_payment_fields_commerce_payment_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
  // Get the default payment checkout pane form.
  $pane_form = commerce_payment_pane_checkout_form($form, $form_state, $checkout_pane, $order);

  // Get the current payment method.
  list($method_id, $rule_name) = explode('|', $pane_form['payment_method']['#default_value']);
  // Add field widget for fieldable payments only.
  if (in_array($method_id, array_keys(commerce_payment_fields_fieldable_payment_methods()))) {
    $payment_method = commerce_payment_method_load($method_id);
    $order = $form_state['order'];
    $transaction = commerce_payment_transaction_new($payment_method['method_id'], $order->order_id);
    $pane_form['payment_details']['transaction'] = array(
      '#type' => 'value',
      '#value' => $transaction,
    );
    $instances = field_info_instances('commerce_payment_transaction', $transaction->payment_method);
    if (!empty($instances)) {
      // Add the field widgets for the profile.
      $pane_form['payment_details']['#parents'] = array($checkout_pane['pane_id']);
      field_attach_form('commerce_payment_transaction', $transaction, $pane_form['payment_details'], $form_state);
    }
  }

  return $pane_form;
}

/**
 * Payment pane: validation callback.
 */
function commerce_payment_fields_commerce_payment_pane_checkout_form_validate($form, &$form_state, $checkout_pane, $order) {
  $pane_id = $checkout_pane['pane_id'];
  // Only attempt fields validation if we actually had the payment transaction on the form.
  if (!empty($form[$pane_id]) && !empty($form_state['values'][$pane_id]) && !empty($form_state['values'][$pane_id]['transaction'])) {
    $pane_form = $form[$pane_id];
    $pane_values = $form_state['values'][$pane_id];
    // Fieldable payment transactions.
    if (!empty($form_state['field'])) {
      $transaction = $pane_values['transaction'];
      // Notify field widgets to validate their data.
      field_attach_form_validate('commerce_payment_transaction', $transaction, $pane_form['payment_details'], $form_state);
      // If there were any errors in the field attach validate process for fields on
      // this checkout pane's payment transaction, fail the checkout pane validation.
      if ($errors = form_get_errors()) {
        foreach ($errors as $field => $error) {
          if (substr($field, 0, strlen($pane_id) + 2) == $pane_id . '][') {
            return FALSE;
          }
        }
      }
    }
  }

  // Call the (core) payment pane validate function.
  return commerce_payment_pane_checkout_form_validate($form, $form_state, $checkout_pane, $order);
}

/**
 * Payment pane: submit callback.
 */
function commerce_payment_fields_commerce_payment_pane_checkout_form_submit($form, &$form_state, $checkout_pane, $order) {
  $pane_id = $checkout_pane['pane_id'];
  // Only attempt fields validation if we actually had the payment transaction on the form.
  if (!empty($form[$pane_id]) && !empty($form_state['values'][$pane_id]) && !empty($form_state['values'][$pane_id]['transaction'])) {
    $pane_form = $form[$pane_id];
    $pane_values = $form_state['values'][$pane_id];
    // Fieldable payment transactions.
    if (!empty($form_state['field'])) {
      $transaction = $transaction = $pane_values['transaction'];
      // Notify field widgets to validate their data.
      field_attach_submit('commerce_payment_transaction', $transaction, $pane_form['payment_details'], $form_state);
    }

    // Provide the transaction object as pane values for the payment submit callback.
    $form_state['values'][$pane_id]['payment_details']['transaction'] = $transaction;
  }

  // Call the (core) payment submit function,
  // passing the transaction object with the fields within.
  return commerce_payment_pane_checkout_form_submit($form, $form_state, $checkout_pane, $order);
}
