<?php

/**
 * @file
 * Contains webpay request and response processing methods.
 */

/**
 * Create new order request and redirect user to payment gateway.
 */
function uc_gp_webpay_request($order) {
  // Set form destination.
  $action = variable_get('uc_gp_url', 'https://test.3dsecure.gpwebpay.com/rb/order.do');
  $order->order_total = sprintf("%01.2f", round($order->order_total, 2));
  watchdog('debug', $order->order_total);
  // Prepare values by specification.
  $values = array(
    'MERCHANTNUMBER' => variable_get('uc_gp_merchant_number', ''),
    'OPERATION' => 'CREATE_ORDER',
    'ORDERNUMBER' => 0,
    'AMOUNT' => str_replace('.', '', $order->order_total),
    'CURRENCY' => uc_gp_get_iso_currency_code(variable_get('uc_currency_code', '')),
    'DEPOSITFLAG' => variable_get('uc_gp_deposit_flag', 0),
    'MERORDERNUM' => $order->order_id,
    'URL' => url('cart/uc_gp/response', array('absolute' => TRUE)),
  );

  // Create new transaction log record so we can get unique transaction ID.
  $write = array();
  foreach ($values as $key => $value) {
    if ($key != 'ORDERNUMBER') {
      $write[drupal_strtolower($key)] = $value;
    }
  }
  drupal_write_record('uc_gp_transaction_request', $write);

  // Update order number.
  $values['ORDERNUMBER'] = $write['ordernumber'];

  // Sign data with private key.
  $values['DIGEST'] = uc_gp_webpay_sign_data($values);

  // Create full URL.
  $url = url($action, array('query' => $values));

  // Redirect user to payment gateway.
  drupal_goto($url);
}

/**
 * Menu callback for handling repsonse from Global Payments response.
 */
function uc_gp_webpay_response() {
  // Define fields that comes from payment gateway.
  $fields = array(
    'OPERATION', 'ORDERNUMBER', 'MERORDERNUM', 'MD', 'PRCODE', 'SRCODE', 'RESULTTEXT',
  );

  // Build incoming data array for singature verification.
  $data = array();
  foreach ($fields as $field) {
    // Exluce empty fields from data signing.
    if (!is_null($_GET[$field])) {
      $data[$field] = $_GET[$field];
    }
  }

  // Check if incoming data are correct.
  if (uc_gp_webpay_verify_data($data, $_GET['DIGEST'])) {
    // Log valid response.
    $write = array();
    foreach ($data as $key => $value) {
      $write[drupal_strtolower($key)] = $value;
    }
    drupal_write_record('uc_gp_transaction_response', $write);

    // Process response.
    // Only if PRCODE and SRCODE are set to 0 payment was succesfull.
    if ($data['PRCODE'] == 0 && $data['SRCODE'] == 0) {
      // Update order state set to paid.
      $order = uc_order_load($data['MERORDERNUM']);
      $order->order_total = round($order->order_total, 2);
      $payment_amount = $order->order_total;
      $context = array(
        'revision' => 'formatted-original',
        'type' => 'amount',
      );
      $options = array(
        'sign' => TRUE,
      );

      // Confirm payment.
      $payment_message = t('Payment using Global Payment Webpay. ORDERNUMBER: @ordernumber', array('@ordernumber' => $data['ORDERNUMBER']));
      uc_payment_enter($order->order_id, 'ug_gp_webpay', $order->order_total, $order->uid, NULL, $payment_message);
      $output = uc_cart_complete_sale($order, FALSE);
      uc_order_update_status($order->order_id, 'payment_received');
      $webpay_amount = array(
        '@amount' => uc_price($payment_amount, $context, $options),
      );
      $payment_submitted = t('Payment of @amount submitted through Global Payment Webpay.', $webpay_amount);
      uc_order_comment_save($order->order_id, 0, $payment_submitted, 'order', 'payment_received');
      $payment_reported = t('Global Payment Webpay reported a payment of @amount.', $webpay_amount);
      uc_order_comment_save($order->order_id, 0, $payment_reported);
      return $output;
    }
    // For validation errors.
    // Non-valid Credit Card show result message to user.
    elseif (in_array($data['PRCODE'], array(28, 30, 1000))) {
      // Set payment pending.
      $webpay_error = t('Global Payment Webpay error of @error.', array('@error' => $data['RESULTTEXT']));
      uc_order_comment_save($order->order_id, 0, $webpay_error);
      drupal_set_message($data['RESULTTEXT']);
      drupal_goto('cart/checkout');
    }
    // Default error handler.
    else {
      drupal_set_message(t('Error when processing payment. If you paid for order please contact site administrator.'), 'error');
      drupal_goto('cart');
    }
  }
  else {
    // Log request to watchdog.
    watchdog('uc_gp', 'Attempt for non-existent order.', array(), WATCHDOG_ERROR);
    // Set message.
    drupal_set_message(t('Recieved wrong data.'), 'error');
    // Redirect user to frontpage with error message.
    return t('Invalid request from Global Payments Webpay. If you paid for order please contact site administrator.');
  }
}
