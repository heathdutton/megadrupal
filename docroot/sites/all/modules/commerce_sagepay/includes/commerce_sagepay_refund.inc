<?php

/**
 * @file
 * commerce_sagepay_refund.inc
 * Provides integration for refunding transactions.
 */

module_load_include('inc', 'commerce_sagepay', 'includes/commerce_sagepay_common');
module_load_include('inc', 'commerce_sagepay', 'includes/commerce_sagepay_utils');

/**
 * Form callback: allows the user to refund a transaction.
 *
 * @param array $form
 *    The form array.
 * @param array $form_state
 *    The form state array
 * @param commerce_order $order
 *    The Commerce Order to process.
 * @param commerce_payment_transaction $transaction
 *    The Commerce Payment Transaction to process.
 *
 * @return mixed
 *  The form array.
 */
function commerce_sagepay_refund_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  $form_state['payment_method'] = $payment_method;

  $balance = commerce_payment_order_balance($order);

  // Assume we are going to want to refund the whole transaction.
  $default_amount = $transaction->amount;

  // Convert the price amount to a user friendly decimal value.
  $default_amount = commerce_currency_amount_to_decimal($default_amount, $transaction->currency_code);

  $description = implode('<br />', array(
    t('Refund: @amount', array('@amount' => commerce_currency_format($transaction->amount, $transaction->currency_code))),
    t('Order balance: @balance', array('@balance' => commerce_currency_format($balance['amount'], $balance['currency_code']))),
  ));

  $form['amount'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Refund amount'),
    '#description'   => check_plain($description),
    '#default_value' => $default_amount,
    '#field_suffix'  => check_plain($transaction->currency_code),
    '#size'          => 16,
  );

  $form = confirm_form($form,
    t('What amount do you want to refund?'),
    'admin/commerce/orders/' . $order->order_id . '/payment',
    '',
    t('Refund'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Validate handler: ensure a valid amount is given.
 */
function commerce_sagepay_refund_form_validate($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];

  // Ensure a positive numeric amount has been entered for capture.
  if (!is_numeric($amount) || $amount <= 0) {
    form_set_error('amount', t('You must specify a positive numeric amount to refund.'));
  }

  // Ensure the amount is less than or equal to the transaction amount.
  if ($amount > commerce_currency_amount_to_decimal($transaction->amount, $transaction->currency_code)) {
    form_set_error('amount', t('You cannot refund more than you collected through SagePay.'));
  }
}

/**
 * Submit handler: process a refund request.
 *
 * @param array $form
 *    The Form array.
 * @param array $form_state
 *    The Form state array.
 */
function commerce_sagepay_refund_form_submit($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];

  // Create a new transaction id for the refund.
  $refund_transaction_id = _commerce_sagepay_vendor_tx_code($transaction, 'REFUND');

  // Set up the request to send to SagePay.
  $query = array(
    'VPSProtocol'         => SAGEPAY_PROTOCOL,
    'TxType'              => 'REFUND',
    'Vendor'              => variable_get(SAGEPAY_SETTING_VENDOR_NAME),
    'VendorTxCode'        => $refund_transaction_id,
    'Amount'              => $amount,
    'Currency'            => $transaction->currency_code,
    'Description'         => t('Refund against order %order_id', array('%order_id' => $transaction->order_id)),
    'RelatedVPSTxId'      => $transaction->remote_id,
    'RelatedVendorTxCode' => $transaction->payload['VendorTxCode'],
    'RelatedSecurityKey'  => $transaction->payload['SecurityKey'],
    'RelatedTxAuthNo'     => $transaction->payload['TxAuthNo'],
  );

  // Determine the url to sent the request to based on the transaction mode.
  switch (variable_get(SAGEPAY_SETTING_TRANSACTION_MODE)) {
    case SAGEPAY_TXN_MODE_LIVE:
      $url = SAGEPAY_SHARED_REFUND_TRANSACTION_LIVE;
      break;

    case SAGEPAY_TXN_MODE_TEST:
      $url = SAGEPAY_SHARED_REFUND_TRANSACTION_TEST;
      break;

    case SAGEPAY_TXN_MODE_SIMULATION:
      $url = SAGEPAY_SHARED_REFUND_TRANSACTION_SIMULATION;
      break;
  }

  // Send the request to SagePay and process the response.
  $query = _commerce_sagepay_array_to_post($query);
  $response = _commerce_sagepay_request_post($url, $query);

  // Update and save the transaction based on the response.
  $transaction->payload[REQUEST_TIME] = $response;

  // Create a new transaction for the refund.
  $refund_transaction = commerce_payment_transaction_new($transaction->payment_method, $transaction->order_id);
  $refund_transaction->instance_id = $transaction->instance_id;
  $refund_transaction->amount = 0 - $amount * 100;
  $refund_transaction->currency_code = $transaction->currency_code;
  $refund_transaction->remote_id = $refund_transaction_id;
  $refund_transaction->payload = $response;
  $refund_transaction->remote_status = SAGEPAY_REMOTE_STATUS_REFUNDED;

  switch ($response['Status']) {
    case 'OK':
      drupal_set_message(t('Payment refunded successfully.'));
      $refund_transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
      break;

    default:
      // Display an error message but leave the transaction pending.
      $refund_transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
      drupal_set_message(t('Transaction refund failed.'), 'error');
      drupal_set_message(check_plain($response['StatusDetail']), 'error');
  }

  $transaction_message = 'Status @status, @statusdetail. ';

  $refund_transaction->message = $transaction_message;
  $refund_transaction->message_variables
    = array(
      '@status'       => $response['Status'],
      '@statusdetail' => $response['StatusDetail'],
    );

  commerce_payment_transaction_save($refund_transaction);
  $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';
}
