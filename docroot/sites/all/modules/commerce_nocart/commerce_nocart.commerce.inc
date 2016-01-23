<?php
/**
 * @file
 * Contains the drupal commerce hooks of the commerce nocart module.
 */

/**
 * Form submit callback: Prepare add-to-cart submission.
 *
 * Constructs and populates a new order with the nocart-flag set. Also ensures
 * that this order will be used in subsequent add-to-cart submit callbacks, even
 * though it is *not* a cart-order (i.e. the order-status does not have the
 * cart-property).
 */
function commerce_nocart_add_to_cart_form_prepare_submit($form, &$form_state) {
  $uid = $form_state['values']['uid'];
  $order = commerce_nocart_order_new($uid, $form_state['commerce_nocart_checkout_page']);
  _commerce_nocart_add_to_cart_order_id($uid, $order->order_id);
}

/**
 * Form submit callback: Finalize add-to-cart submission and redirect checkout.
 */
function commerce_nocart_add_to_cart_form_finalize_submit($form, &$form_state) {
  // Reset enforced order-id returned by our hook_commerce_cart_order_id
  // implementation.
  $uid = $form_state['values']['uid'];
  _commerce_nocart_add_to_cart_order_id($uid, NULL);

  // Redirect to checkout.
  $order_id = $form_state['line_item']->order_id;
  $form_state['redirect'] = 'checkout/' . $order_id;
}

/**
 * Implements hook_commerce_cart_order_id().
 */
function commerce_nocart_commerce_cart_order_id($uid = 0) {
  return _commerce_nocart_add_to_cart_order_id($uid);
}

/**
 * Enforce or reset the order used during the add-to-cart submit callback.
 */
function _commerce_nocart_add_to_cart_order_id($uid, $order_id = NULL) {
  $stored_order_id = &drupal_static(__FUNCTION__, array());

  if (isset($order_id)) {
    $stored_order_id[$uid] = (int) $order_id;
    commerce_cart_order_ids_reset();
  }

  if (isset($stored_order_id[$uid])) {
    return $stored_order_id[$uid];
  }
}

/**
 * Special submit handler for the cancel button to avoid processing orders.
 */
function commerce_nocart_checkout_form_cancel_submit($form, &$form_state) {
  $order = commerce_order_load($form_state['order']->order_id);

  // Set the order status to canceleld.
  $form_state['order'] = commerce_order_status_update($order, 'canceled', TRUE);

  // Skip saving in the status update and manually save here to force a save
  // even when the status doesn't actually change.
  if (variable_get('commerce_order_auto_revision', TRUE)) {
    $form_state['order']->revision = TRUE;
    $form_state['order']->log = t('Customer manually canceled the checkout process.');
  }

  commerce_order_save($form_state['order']);

  $form_state['redirect'] = $form_state['cancel_redirect'];
}
