<?php

/**
 * @file
 * Checkout pane for commerce_webform module
 * This validates an order to ensure webform mandatory productfields
 * are still in the order.
 */

/**
 * Checkout pane callback
 * Validate the order to check all mandatory items
 * are still in the order at checkout.
 */
function commerce_webform_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
  _commerce_webform_validate_order($order);
}

/**
 * This is a check to make sure the given order still
 * has webform specified mandatory products in it.
 */
function _commerce_webform_validate_order($order) {
  // Check order is a commerce_webform order.
  if (commerce_webform_is_commerce_webform_order($order)) {
    // Loop through productfields check for mandatory status.
  }
}
