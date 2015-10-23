<?php

/**
 * @file
 * Verify IPN updates from BitPay and update relevant transactions.
 */

/**
 * Interpret IPN updates from BitPay and update relevant payment transactions.
 */
function commerce_bitpay_verify_ipn($order, $transaction) {
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  $payment_method['settings'] += commerce_bitpay_default_settings();

  libraries_load('php-client');

  $json = bpVerifyNotification($payment_method['settings']['apiKey']);

  if (is_null($json) || is_string($json) || !isset($json['posData']) || !is_numeric($json['posData'])) {
    switch ($json) {
      case 'No post data':
        watchdog('commerce_bitpay', 'IPN POST had no data.', array(), WATCHDOG_WARNING);
        break;

      case 'no posData':
        watchdog('commerce_bitpay', 'No IPN posData value was provided.', array(), WATCHDOG_WARNING);
        break;

      case 'authentication failed (bad hash)':
        watchdog('commerce_bitpay', 'IPN verification failed (bad hash).', array(), WATCHDOG_WARNING);
        break;

      default:
        watchdog('commerce_bitpay', 'IPN posted something strange: @string', array('@string' => $json));
    }
    return FALSE;
  }

  watchdog('commerce_bitpay', 'IPN received for order @order_id, transaction @trans_id.', array('@trans_id' => $transaction->transaction_id, '@order_id' => $order->order_id), WATCHDOG_DEBUG, l(t('view order payments'), 'admin/commerce/orders/' . $order->order_id . '/payment'));

  commerce_bitpay_update_transaction($order, $transaction, $json);
}
