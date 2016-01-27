<?php

/**
 * @file
 * Administrative page callbacks for the Commerce PayPal EC module.
 */

/**
 * Process a Capture Authorization via API call and update the transaction with
 * the response received. This make possible to process captures directly from
 * Drupal Commerce.
 *
 * @param object $order
 *   The order object for which the transaction belongs to.
 * @param object $commerce_payment_transaction
 *   The commerce_payment_transaction object to process.
 */
function commerce_paypal_ec_transaction_capture($order, $commerce_payment_transaction) {
  // if transaction status is success then the capture had been executed
  if ($commerce_payment_transaction->status == 'success') {
    drupal_set_message(t('The payment has been captured before.'), 'warning');
    drupal_goto(url("admin/commerce/orders/{$order->order_id}/payment/{$commerce_payment_transaction->transaction_id}/view"));
  }

  // wrap commerce_payment_transaction to simplify data access
  $transaction_wrapper = entity_metadata_wrapper('commerce_payment_transaction', $commerce_payment_transaction);
  // amount of the order
  $amount = $transaction_wrapper->order->commerce_order_total->amount->value();
  // get payment settings
  $settings = _commerce_paypal_ec_get_transaction_payment_settings($transaction_wrapper);
  // determine the currency code
  $currency_code = _commerce_paypal_ec_lookup_currency_code($order, $settings);

  $request_params = array(
    'AUTHORIZATIONID' => $transaction_wrapper->remote_id->value(),
    'AMT' => commerce_paypal_ec_format_amount($amount, $currency_code),
    'COMPLETETYPE' => 'Complete',
    'CURRENCYCODE' => $currency_code,
  );

  $response = commerce_paypal_ec_api_call("DOCapture", $request_params, $settings);

  if ($response['ACK'] == 'Success') {
    if ($response['PAYMENTSTATUS'] == 'Completed') {
      // add order_id to response array
      $response['order_id'] = $transaction_wrapper->order->order_id->value();
      // convert response to IPN format and store in paypal_ipn
      $ipn = commerce_paypal_ec_response_capture_to_ipn($response);
      //commerce_paypal_ipn_save($ipn);
      // Process the response. Instead of use the IPN request we use the DOCapture
      // response and we can have the Payment info instantly
      commerce_paypal_ec_paypal_ipn_process($order, $payment_method, $ipn);
      drupal_set_message(t('The Capture was successful processed.'));
    }
    else {
      // The call could be success but not completing the transaction capture.
      // An example is when the store PayPal account receive a transaction
      // in different currency than the accepted and has not configured to
      // automatically do the conversion. As a result the capture will fail.
      drupal_set_message(t('The Catpure failed: !error', array('!error' => $response['PENDINGREASON'])), 'warning');
    }
  }
  else {
    drupal_set_message(t('The Catpure failed: !error', array('!error' => $response['L_LONGMESSAGE0'])), 'warning');
  }

  // redirect to the transaction view
  drupal_goto(url("admin/commerce/orders/{$order->order_id}/payment/{$commerce_payment_transaction->transaction_id}/view"));
}

/**
 * Process a Refund via RefundTransaction API call
 */
function commerce_paypal_ec_transaction_refund($order, $commerce_payment_transaction) {
  // if transaction status is success then the capture had been executed
  if ($commerce_payment_transaction->status == 'paypal_ec_refund') {
    drupal_set_message(t('The payment has been refund before.'), 'warning');
    drupal_goto(url("admin/commerce/orders/{$order->order_id}/payment/{$commerce_payment_transaction->transaction_id}/view"));
  }

  // wrap commerce_payment_transaction to simplify data access
  $transaction_wrapper = entity_metadata_wrapper('commerce_payment_transaction', $commerce_payment_transaction);
  // amount of the order
  $amount = $transaction_wrapper->order->commerce_order_total->amount->value();
  // get payment settings
  $settings = _commerce_paypal_ec_get_transaction_payment_settings($transaction_wrapper);
  // determine the currency code
  $currency_code = _commerce_paypal_ec_lookup_currency_code($order, $settings);

  $request_params = array(
    'TRANSACTIONID' => $transaction_wrapper->remote_id->value(),
    'REFUNDTYPE' => 'Full',
    'AMT' => commerce_paypal_ec_format_amount($amount, $currency_code),
    'COMPLETETYPE' => 'Complete',
    'CURRENCYCODE' => $currency_code,
  );

  $response = commerce_paypal_ec_api_call("RefundTransaction", $request_params, $settings);

  if ($response['ACK'] == 'Success') {
    if ($response['REFUNDSTATUS'] == 'Instant') {
      // add order_id to response array
      $response['order_id'] = $transaction_wrapper->order->order_id->value();
      $ipn = commerce_paypal_ec_response_refund_to_ipn($response);
      // Process the response. Instead of use the IPN request we use the
      // RefundTransaction response response and we can have the Payment info
      // instantly
      commerce_paypal_ec_paypal_ipn_process($order, $payment_method, $ipn);
      drupal_set_message(t('The Refund was successful processed.'));
    }
    else {
      // in case the refund stay pending for any reason
      drupal_set_message(t('The Refund failed: !error', array('!error' => $response['PENDINGREASON'])), 'warning');
    }
  }
  else {
    drupal_set_message(t('The Refund failed: !error', array('!error' => $response['L_LONGMESSAGE0'])), 'warning');
  }

  // redirect to the transaction view
  drupal_goto(url("admin/commerce/orders/{$order->order_id}/payment/{$commerce_payment_transaction->transaction_id}/view"));
}

/**
 * Form callback: confirmation form for refunding a transaction.
 *
 * @param $transaction
 *   The payment transaction object to be refunded.
 *
 * @see confirm_form()
 */
function commerce_paypal_ec_payment_transaction_operation_form($form, &$form_state, $operation, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;
  $form_state['transaction_operation'] = $operation;

  // Load and store the payment method.
  $payment_method = commerce_payment_method_load($transaction->payment_method);
  $form_state['payment_method'] = $payment_method;
  $form['#submit'][] = 'commerce_paypal_ec_payment_transaction_operation_form_submit';

  $form = confirm_form($form,
    t("Are you sure you want to {$operation} this transaction?"),
    '',
    '<p>' . t('@amount paid via %method on @date. This operation cannot be undone.', array('@amount' => commerce_currency_format($transaction->amount, $transaction->currency_code), '%method' => $payment_method['title'], '@date' => format_date($transaction->created, 'short'))) . '</p>',
    t($operation),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for commerce_payment_payment_transaction_delete_form_submit().
 */
function commerce_paypal_ec_payment_transaction_operation_form_submit($form, &$form_state) {
  $order = $form_state['order'];
  $transaction = $form_state['transaction'];
  $operation = strtolower($form_state['transaction_operation']);

  switch ($operation) {
    case 'refund' :
      commerce_paypal_ec_transaction_refund($order, $transaction);
      break;
    case 'capture' :
      commerce_paypal_ec_transaction_capture($order, $transaction);
      break;
  }
}

/**
 * Get the transaction currency code.
 */
function _commerce_paypal_ec_get_transaction_payment_settings($transaction_wrapper) {
  // load the payment method to get settings
  $payment_instance_id = $transaction_wrapper->order->value()->data['payment_method'];
  $payment_method = commerce_payment_method_instance_load($payment_instance_id);
  return $payment_method['settings'];
}
