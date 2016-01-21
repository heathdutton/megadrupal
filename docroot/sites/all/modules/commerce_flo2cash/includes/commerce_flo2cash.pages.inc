<?php

/**
 * @file
 * Menu callbacks for Commerce Flo2Cash.
 */

/**
 * Menu callback for MNS check.
 */
function commerce_flo2cash_mns_check($order, $redirect_key) {
  $post_data = array_map('check_plain', $_POST);
  // Have we been passed a valid key?
  if ($redirect_key !== $order->data['payment_redirect_key']) {
    $msg = 'MNS: The payment redirect key did not match the order. Expected %key.';
    $args = array(
      '%key' => $order->data['payment_redirect_key'],
    );
    watchdog('commerce_flo2cash', $msg, $args, WATCHDOG_WARNING);
    drupal_not_found();
  }
  // Have we been posted some data?
  elseif (empty($post_data)) {
    $msg = 'MNS: No POST data received with this request.';
    watchdog('commerce_flo2cash', $msg, WATCHDOG_WARNING);
    drupal_not_found();
  }
  // Have we been posted the right data?
  elseif (empty($post_data['transaction_id'])) {
    // @todo There are better ways to check this more thoroughly.
    $msg = 'MNS: POST data did not match expected format: !post_data';
    $args =  array(
      '!post_data' => '<pre>' . print_r($post_data, TRUE) . '</pre>',
    );
    watchdog('commerce_flo2cash', $msg, $args, WATCHDOG_WARNING);
    drupal_not_found();
  }

  $order_id = $order->order_id;
  $transaction_id = $post_data['transaction_id'];
  $payment_id = $order->data['payment_method'];
  $payment_method = commerce_payment_method_instance_load($payment_id);
  $notification_url = $payment_method['settings']['web2pay_mns_url'];

  // @todo Split curl into different function.
  $post_string = http_build_query($post_data);
  $post_string .= '&cmd=_xverify-transaction';

  // Initializing curl.
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, $notification_url);
  curl_setopt($ch, CURLOPT_POST, 1);
  curl_setopt($ch, CURLOPT_POSTFIELDS, $post_string);
  curl_setopt($ch, CURLOPT_HEADER, 0);
  curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
  curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
  curl_setopt($ch, CURLOPT_TIMEOUT, 100);
  curl_setopt($ch, CURLOPT_VERBOSE, 1);
  curl_setopt($ch, CURLOPT_NOPROGRESS, 0);
  $response = curl_exec($ch);
  curl_close($ch);

  // Verify the submitted data via Flo2Cash MNS.
  if ($response == 'VERIFIED') {
    // Credit card transaction must be successful. (status code of 2)
    // @todo define this as a constant
    if ($post_data['transaction_status'] != FLO2CASH_STATUS_SUCCESS) {
      $msg = 'MNS Check: Payment attempt failed for transaction @id: @text';
      $args = array(
        '@id' => $transaction_id,
        '@text' => $post_data['response_text'],
      );
      watchdog('commerce_flo2cash', $msg, $args, WATCHDOG_WARNING);
      return '';
    }

    // Calculate total that payment was received for.
    $total = 0;
    $i = 1;
    while (isset($post_data['item_price' . $i])) {
      $price = $post_data['item_price' . $i];
      $qty = $post_data['item_qty' . $i];
      $total = bcadd($total, bcmul($price, $qty, 2), 2);
      $i++;
    }

    // @todo Transaction in a different function.
    // Create a new transaction.
    $transaction = commerce_payment_transaction_new('flo2cash', $order_id);
    $transaction->instance_id = $payment_method['instance_id'];
    $transaction->remote_id = $transaction_id;
    // Assume currency used is that of the order.
    $currency_code = $order->commerce_order_total[LANGUAGE_NONE][0]['currency_code'];
    $amount = commerce_currency_decimal_to_amount($total, $currency_code);
    $transaction->amount = $amount;
    $transaction->currency_code = $currency_code;
    // Set status, message, and payload.
    $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
    $transaction->remote_status = $post_data['transaction_status'];
    $transaction->message = $post_data['response_text'];
    $transaction->payload = $post_data;

    commerce_payment_transaction_save($transaction);

    $msg = 'MNS: Flo2Cash payment of $@total verified for order @order.';
    $args = array(
      '@total' => $total,
      '@order' => $order_id,
    );
    $link = l('View order', 'admin/commerce/orders/' . $order_id);
    watchdog('commerce_flo2cash', $msg, $args, WATCHDOG_NOTICE, $link);
  }
  else {
    $msg = 'MNS: The transaction failed to verify. POST data and response: !post_data !response';
    $args = array(
      '!post_data' => '<pre>' . check_plain(print_r($_POST, TRUE)) . '</pre>',
      '!response' => '<pre>' . check_plain(print_r($response, TRUE)) . '</pre>',
    );
    watchdog('commerce_flo2cash', $msg, $args, WATCHDOG_ERROR);
  }

  return '';
}

/**
 * Menu callback for return URL.
 */
function commerce_flo2cash_return_url($order, $redirect_key) {
  $order_id = $order->order_id;
  $post_data = array_map('check_plain', $_POST);

  // Have we been passed a valid key?
  if ($redirect_key !== $order->data['payment_redirect_key']) {
    $msg = 'Web2Pay Return: The payment redirect key did not match the order. Expected %key.';
    $args = array(
      '%key' => $order->data['payment_redirect_key'],
    );
    watchdog('commerce_flo2cash', $msg, $args, WATCHDOG_WARNING);
    drupal_not_found();
  }
  // Have we been posted some data?
  elseif (empty($post_data)) {
    $msg = 'Web2Pay Return: No POST data received with this request.';
    watchdog('commerce_flo2cash', $msg, WATCHDOG_WARNING);

    drupal_set_message(t('No data was returned from the payment gateway.'));
    drupal_goto('checkout/' . $order_id . '/payment/back/' . $redirect_key);
  }
  // Have we been passed valid POST data with a status?
  elseif (empty($post_data['txn_status'])) {
    // @todo There are better ways to check this more thoroughly.
    $msg = 'Web2Pay Return: POST data did not match expected format: !post_data';
    $args =  array(
      '!post_data' => '<pre>' . print_r($post_data, TRUE) . '</pre>',
    );
    watchdog('commerce_flo2cash', $msg, $args, WATCHDOG_ERROR);

    drupal_set_message(t('Data was returned from the payment gateway was not valid.'));
    drupal_goto('checkout/' . $order_id . '/payment/back/' . $redirect_key);
  }
  // Was the transaction successful?
  elseif ($post_data['txn_status'] != FLO2CASH_STATUS_SUCCESS) {
    $msg = 'Web2Pay Return: Transaction was declined: !post_data';
    $args =  array(
      '!post_data' => '<pre>' . print_r($post_data, TRUE) . '</pre>',
    );
    watchdog('commerce_flo2cash', $msg, $args, WATCHDOG_ERROR);

    $msg = 'The payment was not successful: <strong>@error</strong>.';
    $args =  array(
      '@error' => $post_data['response_text'],
    );
    drupal_set_message(t($msg, $args, 'error'));
    drupal_goto('checkout/' . $order_id . '/payment/back/' . $redirect_key);
  }

  // Payment was apparently a success, so we log it as such here.
  $msg = 'Web2Pay Return: @response_text, ID: @txn_id, Receipt: @receipt_no.';
  $args = array(
    '@response_text' => $post_data['response_text'],
    '@txn_id' => $post_data['txn_id'],
    '@receipt_no' => $post_data['receipt_no'],
  );
  watchdog('commerce_flo2cash', $msg, $args, WATCHDOG_NOTICE);

  // Now we need to wait for the MNS check to complete so we send the
  // user to a holding page.
  drupal_goto('checkout/' . $order_id . '/flo2cash/verify/' . $redirect_key . '/' . $post_data['txn_id']);
}

/**
 * Menu callback for payment verification
 */
function commerce_flo2cash_verify_payment($order, $redirect_key, $txn_id) {
  $order_id = $order->order_id;

  // Have we been passed a valid key?
  if ($redirect_key !== $order->data['payment_redirect_key']) {
    $msg = 'Verify: The payment redirect key did not match the order. Expected %key.';
    $args = array(
      '%key' => $order->data['payment_redirect_key'],
    );
    watchdog('commerce_flo2cash', $msg, $args, WATCHDOG_WARNING);
    drupal_not_found();
  }

  // Load transactions for this order
  $args = array('order_id' =>  $order_id);
  $payments = commerce_payment_transaction_load_multiple(array(), $args);

  // Find one that matches the given transaction ID.
  foreach ($payments as $payment) {
    if ($payment->remote_id == $txn_id) {
      // Continue with normal commerce processing.
      drupal_goto('checkout/' . $order_id . '/payment/return/' . $redirect_key);
    }
  }

  // @todo Use AJAX to bail after 20 seconds or so.
  $base_path = drupal_get_path('module', 'commerce_flo2cash');
  drupal_add_js($base_path . '/js/commerce_flo2cash.js');

  return t('<div class="commerce-flo2cash-verify">Waiting for a response from the server. If you are not redirected to the confirmation page within 20 seconds, please contact the store administrator for manual intervention.</div>');
}
