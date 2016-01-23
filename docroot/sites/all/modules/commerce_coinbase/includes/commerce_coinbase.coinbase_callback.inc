<?php

/**
 * @file
 * Transaction callbacks from Coinbase.
 */

/**
 * Process payment information from Coinbase.
 *
 * @link https://coinbase.com/docs/merchant_tools/callbacks Callbacks @endlink
 */
function commerce_coinbase_process_callback($order, $transaction) {
  /*
  At this point, the menu handler should have already checked the validity
  of the secret hash in the URL against the transaction. The data processed
  in this function should be considered safe if you trust the security of
  SSL on your callback URL.
  */
  watchdog('commerce_coinbase', 'Callback received for order @order_id, transaction @trans_id.', array('@trans_id' => $transaction->transaction_id, '@order_id' => $order->order_id), WATCHDOG_DEBUG, l(t('view order payments'), 'admin/commerce/orders/' . $order->order_id . '/payment'));

  $post_body = json_decode(file_get_contents('php://input'));
  if (empty($post_body)) {
    watchdog('commerce_coinbase', 'Transaction callback was empty.');
    return;
  }
  if ($post_body->order->custom != $transaction->transaction_id) {
    watchdog('commerce_coinbase', 'Possible malicious callback posted incorrect custom variable.');
    return;
  }
  if (empty($post_body->order->status)) {
    watchdog('commerce_coinbase', 'Invalid transaction callback.');
  }
  else {
    // Update the Drupal order based on the Coinbase order.
    switch ($post_body->order->status) {
      case 'completed':
        commerce_checkout_complete($order);
        $transaction->message = t('Payment received. Confirmations pending.');
        $transaction->remote_id = $post_body->order->transaction->id;
        break;

      case 'canceled':
        $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
        $transaction->message = t('Payment was canceled.');
        break;

      default:
        watchdog('commerce_coinbase', 'Unknown payment status for transaction @id: @status', array('@id' => $transaction->transaction_id, '@status' => $post_body->order->status), WATCHDOG_WARNING);
        return;
    }
    $transaction->payload = $post_body;
    commerce_payment_transaction_save($transaction);

    // Update the Drupal transaction based on the Coinbase transaction.
    commerce_coinbase_update_transaction($transaction, $post_body->order->transaction->id);
  }
}
