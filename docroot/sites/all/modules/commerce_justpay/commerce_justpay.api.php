<?php

/**
 * @file
 * Hooks provided by the Commerce Justpay module.
 */

/**
 * Allows the payment method to be modified just prior to a charge being made.
 *
 * @param array $payment_method
 *   The payment method about to be used to pay for the order
 * @param array $form
 *   The just submitted single page checkout form
 * @param array $form_state
 *   The submitted form state
 *
 * @see commerce_justpay_form_submit()
 */
function hook_commerce_justpay_payment_method_alter(&$payment_method, &$form, &$form_state) {
  $payment_method['settings']['mode'] = 'cvn_xml';
}


/**
 * Allows the order to be altered just prior to it being processed.
 *
 * @param object $order
 *   The order object about to be paid for
 * @param array $form
 *   The just submitted single page checkout form
 * @param array $form_state
 *   The submitted form state
 *
 * @see commerce_justpay_form_submit()
 */
function hook_commerce_justpay_order_alter($order, &$form, &$form_state) {
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $order_wrapper->field_invoice_reference = $form_state['values']['invoice_ref'];
}

/**
 * Allows the offsite redirect form to be altered specifically for this module.
 *
 * @param array $form
 *   The just submitted single page checkout form
 * @param object $order
 *   The order object about to be paid for
 *
 * @see commerce_justpay_offsite_payment_form()
 */
function hook_commerce_justpay_CALLBACK_alter(&$form, $order) {
  // This example modifies the Paypal WPS return URLS.
  $form['return']['#value'] = str_replace('checkout/' . $order->order_id . '/payment', 'justpay/offsite', $form['return']['#value']);
  $form['cancel_return']['#value'] = str_replace('checkout/' . $order->order_id . '/payment', 'justpay/offsite', $form['cancel_return']['#value']);
}
