<?php
/**
 * @file
 * Provides the ability to add a 'review payment method' to the Drupal Commerce
 * checkout.
 */

/**
 * Checkout pane callback: return review data.
 *
 * Returns the cart contents review data for the Review checkout pane.
 */
function commerce_checkout_pane_payment_pane_review($form, $form_state, $checkout_pane, $order) {
  // Use default payment method if order was not saved yet.
  if (!isset($order->data['payment_method'])) {
    $order->data['payment_method'] = $form['commerce_payment']['payment_method']['#default_value'];
  }

  // Load the payment method.
  $payment_method = commerce_payment_method_load(substr($order->data['payment_method'], 0, strpos($order->data['payment_method'], '|', 0)));

  return theme('commerce_checkout_pane_payment_review', array(
    'order' => $order,
    'payment' => $payment_method,
  ));
}
