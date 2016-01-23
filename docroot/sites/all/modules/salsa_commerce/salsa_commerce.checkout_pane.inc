<?php

/**
 * @file Contains checkout panes for the salsa_commerce module.
 */

/**
 * Checkout pane form callback.
 *
 * Redirects user to the configured redirect link of the salsa event.
 */
function salsa_commerce_redirect_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
  if (!empty($order->data['redirect_path'])) {
    drupal_goto($order->data['redirect_path']);
  }
}
