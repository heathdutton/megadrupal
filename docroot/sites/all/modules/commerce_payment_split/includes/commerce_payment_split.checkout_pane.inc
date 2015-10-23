<?php
/**
 * @file
 * Contain the logic code for commerce_payment_split pane
 */

function commerce_payment_split_checkout_form($form, &$form_state, $checkout_pane, $order) {
  // Show a input for user enter the amount. Default is the $balance left
  $pane_form = array();
  $balance = commerce_payment_split_order_balance($order);
  $pane_form['payment_split']['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Amount'),
    '#size' => 30,
    '#default_value' => commerce_currency_amount_to_decimal($balance['amount'], $balance['currency_code']),
  );
  $pane_form['payment_split']['currency_code'] = array(
    '#type' => 'value',
    '#value' => $balance['currency_code'],
  );
  return $pane_form;
}

function commerce_payment_split_checkout_form_validate($form, &$form_state, $checkout_pane, $order) {
  $pane_id = $checkout_pane['pane_id'];
  $values = $form_state['values'][$pane_id]['payment_split'];
  // Change value
  $values['amount'] = commerce_currency_decimal_to_amount($values['amount'], $values['currency_code']);
  $order_total = commerce_payment_split_order_balance($order);
  if ($values['amount'] > $order_total['amount']) {
    drupal_set_message(t('You cannot prepaid amount larger than total amount.'), 'error');
    return FALSE;
  }
  return TRUE;
}

function commerce_payment_split_checkout_form_submit($form, &$form_state, $checkout_pane, $order) {
  $pane_id = $checkout_pane['pane_id'];
  $values = $form_state['values'][$pane_id]['payment_split'];
  $values['amount'] = commerce_currency_decimal_to_amount($values['amount'], $values['currency_code']);
  commerce_payment_split_apply($values, $order);
}
