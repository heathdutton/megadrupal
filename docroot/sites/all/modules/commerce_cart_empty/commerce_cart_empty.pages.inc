<?php
/**
 * @file
 * Menu callbacks for the Commerce Cart Empty module.
 */

/**
 * Menu callback to clear the current user's cart.
 */
function commerce_cart_empty_cart_clear() {
  global $user;

  // Load the current shopping cart order.
  $order = commerce_cart_order_load($user->uid);

  // Delete the order, if found.
  if (!empty($order)) {
    commerce_order_delete($order->order_id);
    drupal_set_message(t('The cart has been emptied.'));
  }
  else {
    drupal_set_message(t('The cart was already empty.'));
  }

  drupal_goto();
}
