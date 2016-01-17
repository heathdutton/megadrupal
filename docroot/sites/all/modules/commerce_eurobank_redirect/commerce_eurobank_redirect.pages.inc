<?php

/**
 * @file
 * Eurobank EFG payment menu items.
 */

define('AB_SUCCESFUL_TRANSACTION_AUTHORIZED', 'AUTHORIZED');
define('AB_SUCCESFUL_TRANSACTION_CAPTURED', 'CAPTURED');
define('AB_ERROR_TRANSACTION', 'ERROR');
define('AB_REFUSED_TRANSACTION', 'REFUSED');
define('AB_TRANSACTION_CANCELED', 'CANCELED');

/**
 * Hook function for succesful order.
 */
function commerce_eurobank_redirect_complete() {
  // Get the _POST data from eurobank.
  $data = $_POST;
  $post_data_array = array();

  if (isset($data['mid'])) {
    $post_data_array[] = $data['mid'];
  }
  if (isset($data['orderid'])) {
    $post_data_array[] = $data['orderid'];
  }
  if (isset($data['status'])) {
    $post_data_array[] = $data['status'];
  }
  if (isset($data['orderAmount'])) {
    $post_data_array[] = $data['orderAmount'];
  }
  if (isset($data['currency'])) {
    $post_data_array[] = $data['currency'];
  }
  if (isset($data['paymentTotal'])) {
    $post_data_array[] = $data['paymentTotal'];
  }
  if (isset($data['message'])) {
    $post_data_array[] = $data['message'];
  }
  if (isset($data['riskScore'])) {
    $post_data_array[] = $data['riskScore'];
  }
  if (isset($data['payMethod'])) {
     $post_data_array[] = $data['payMethod'];
  }
  if (isset($data['txId'])) {
    $post_data_array[] = $data['txId'];
  }
  if (isset($data['paymentRef'])) {
    $post_data_array[] = $data['paymentRef'];
  }

  $post_data_array[] = variable_get('eurobank_shared_secret_key');

  $post_digest = $data['digest'];

  $post_data = implode("", $post_data_array);
  $digest = base64_encode(sha1($post_data, TRUE));

  $order_id = isset($data['orderid']) ? check_plain(drupal_substr($data['orderid'], 0, -10)) : '';
  watchdog('commerce_eurobank', 'Receiving new order notification for order @order_id.', array('@order_id' => $order_id));

  $order = commerce_static_checkout_url_load_order_or_die($order_id);

  $payment_status = isset($data['status']) ? check_plain($data['status']) : '';
  $delta_pay_id = isset($data['txId']) ? check_plain($data['txId']) : '';
  $payment_amount = isset($data['orderAmount']) ? check_plain($data['orderAmount']) : '';
  $payment_amount = str_replace(',', '.', $payment_amount);
  $payment_currency_code = isset($data['currency']) ? check_plain($data['currency']) : '';

  if ($payment_status != AB_SUCCESFUL_TRANSACTION_AUTHORIZED && $payment_status != AB_SUCCESFUL_TRANSACTION_CAPTURED && $post_digest != $digest) {
    commerce_static_checkout_url_redirect_with_error('eurobank', $order, 'Wrong POST DATA', TRUE);
  }

  $transaction = commerce_payment_transaction_new('commerce_eurobank_redirect', $order_id);
  $transaction->instance_id = $delta_pay_id;
  $transaction->amount = floatval($payment_amount) * 100;
  $transaction->currency_code = $payment_currency_code;
  $transaction->payload[REQUEST_TIME] = $data;
  $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
  $transaction->remote_id = $delta_pay_id;
  $transaction->remote_status = $payment_status;
  $transaction->message = t('Payment was successful');
  $result = commerce_payment_transaction_save($transaction);
  commerce_checkout_complete($order);

  commerce_static_checkout_url_redirect_with_success('eurobank');
}

/**
 * Hook function for order with failure.
 */
function commerce_eurobank_redirect_error() {
  // Get the _POST data from eurobank.
  $data = $_POST;

  $order_id = isset($data['orderid']) ? check_plain(drupal_substr($data['orderid'], 0, -10)) : '';

  $order = commerce_static_checkout_url_load_order_or_die($order_id);

  $payment_status = isset($data['status']) ? check_plain($data['status']) : '';
  // $delta_pay_id = $_POST['DeltaPayId'];
  // NOT RETURNED ON CANCEL!
  $delta_pay_id = -1;
  $error_message = isset($data['message']) ? check_plain($data['message']) : '';

  if ($payment_status != AB_ERROR_TRANSACTION && $payment_status != AB_REFUSED_TRANSACTION && $payment_status != AB_TRANSACTION_CANCELED) {
    commerce_static_checkout_url_redirect_with_error('eurobank', $order, 'Wrong POST DATA', TRUE);
  }

  $transaction = commerce_payment_transaction_new('commerce_eurobank_redirect', $order_id);
  $transaction->instance_id = -1;
  $transaction->payload[REQUEST_TIME] = $data;
  $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
  $transaction->remote_id = $delta_pay_id;
  $transaction->remote_status = $payment_status;

  if ($payment_status == 'CANCELED') {
    $error_message = t('Payment was cancelled');
  }

  $transaction->message = $error_message;

  $result = commerce_payment_transaction_save($transaction);

   if ($payment_status == 'CANCELED') {
    commerce_static_checkout_url_redirect_with_cancel('eurobank', $order);
  }
  else {
    commerce_static_checkout_url_redirect_with_error('eurobank', $order, $error_message);
  }

}
