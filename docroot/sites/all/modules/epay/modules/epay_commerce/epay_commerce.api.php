<?php

/**
 * @file
 * Hooks provided by the ePay Commerce module.
 */

/**
 * Allows modules to alter the order status definitions of other modules.
 *
 * @param $order_statuses
 *   An array of order statuses defined by enabled modules.
 *
 * @see hook_commerce_order_status_info()
 */
function hook_commerce_order_status_info_alter(&$order_statuses) {
  $order_statuses['completed']['title'] = t('Finished');
}

/**
 * Allows modules to react on epay commerce failing to capture remaining
 * amount on an order.
 *
 * @param $order
 *    A fully loaded Drupal Commerce order entity.
 * @param $txnid
 *    The transaction id from ePay on the transaction that failed.
 */
function hook_epay_commerce_captured_remaining_amount_fail($order, $txnid) {
  // Example - change order status when capture fails.
  $order->status = 'epay_commerce_capture_fail';
  commerce_order_save($order);
}
