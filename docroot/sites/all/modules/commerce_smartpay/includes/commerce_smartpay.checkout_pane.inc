<?php

/**
 * @file
 * Checkout pane callback functions.
 */

/**
 * Implements base_checkout_form().
 */
function commerce_smartpay_pane_checkout_form($form, $form_state, $checkout_pane, $order) {
  if (isset($order->data['commerce_smartpay_form'])) {
    $checkout_form['commerce_smartpay_pane_field_display'] = array(
      '#markup' => commerce_smartpay_payment_details($order),
    );

    return $checkout_form;
  }
}
