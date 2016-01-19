<?php

/**
 * @file
 * Hooks provided by the commerce_intuit module.
 */

/**
 * Alter the default value for a date argument.
 *
 * @param array $txn_array
 *   The argument array.
 * @param object $order
 *   The default value created by the argument handler.
 */
function hook_commerce_intuit_transaction_alter(&$txn_array, &$order) {
  if ($order->order_id % 1000 == 0) {
    // Every 1000 order has a 10% discount.
    $txn_array['amount'] = $txn_array['amount'] - ($txn_array['amount'] * 10 / 100);
  }
}
