<?php

/**
 * Commerce Heidelpay CheckOut
 */

/**
 * Menu callback: Redirect URL for Heidelpay
 */
function commerce_heidelpay_hco_redirect($order_id, $payment_redirect_key) {
  $response = $_POST;
  // Well, that shouldn't be necessary, but the response data encoding appears
  // to be incorrect
  commerce_heidelpay_response_prepare($response);

  $order = commerce_order_load($order_id);
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $currency_code = $order_wrapper->commerce_order_total->currency_code->value();
  $amount = $order_wrapper->commerce_order_total->amount->value();

  $payment_method = commerce_payment_method_load('heidelpay');

  watchdog('heidelpay', 'Response from Heidelpay: !data', array('!data' => '<pre>' . check_plain(print_r($response, 1)) . '</pre>'));

  // Create a new payment transaction for the order.
  $transaction = commerce_payment_transaction_new('heidelpay', $order->order_id);
  //$transaction->instance_id = $payment_method['instance_id'];

  $transaction->remote_id = $response['IDENTIFICATION_UNIQUEID'];
  $transaction->amount = commerce_currency_decimal_to_amount($response['PRESENTATION_AMOUNT'], $response['PRESENTATION_CURRENCY']);
  $transaction->currency_code = $response['PRESENTATION_CURRENCY'];
  $transaction->payload = $response;
  $transaction->remote_status = $response['PROCESSING_STATUS_CODE'];

  switch ($response['PROCESSING_RESULT']) {
    case 'ACK':
      $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
      $transaction->message = t('The payment has completed.');

      $url = url('checkout/' . $order->order_id . '/payment/return/' . $order->data['payment_redirect_key'], array('absolute' => TRUE));
      break;
    default:
      $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
      $transaction->message = t("The payment has failed.");

      $url = url('checkout/' . $order->order_id . '/payment/back/' . $order->data['payment_redirect_key'], array('absolute' => TRUE));
      break;
  }

  // Save the transaction information.
  commerce_payment_transaction_save($transaction);
  watchdog('heidelpay', 'Payment processed for Order @order_number with ID @txn_id.', array('@txn_id' => $transaction->remote_id, '@order_number' => $order->order_number), WATCHDOG_INFO);

  print $url;
}
