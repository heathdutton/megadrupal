<?php

/**
 * @file
 * commerce_sagepay_release.inc
 * Functions for releasing deferred payments.
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
 *    The form array.
 */
function commerce_sagepay_release_form($form, &$form_state, $order, $transaction) {
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
    t('Release: @amount', array('@amount' => commerce_currency_format($transaction->amount, $transaction->currency_code))),
    t('Order balance: @balance', array('@balance' => commerce_currency_format($balance['amount'], $balance['currency_code']))),
  ));

  $form['amount'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Release amount'),
    '#description'   => check_plain($description),
    '#default_value' => $default_amount,
    '#field_suffix'  => check_plain($transaction->currency_code),
    '#size'          => 16,
  );

  $form = confirm_form($form,
    t('How much do you want to release?'),
    'admin/commerce/orders/' . $order->order_id . '/payment',
    '',
    t('Release'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Validate handler: ensure a valid amount is given.
 */
function commerce_sagepay_release_form_validate($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];

  // Ensure a positive numeric amount has been entered for capture.
  if (!is_numeric($amount) || $amount <= 0) {
    form_set_error('amount', t('You must specify a positive numeric amount to release.'));
  }

  // Ensure the amount is less than or equal to the transaction amount.
  if ($amount > commerce_currency_amount_to_decimal($transaction->amount, $transaction->currency_code)) {
    form_set_error('amount', t('You cannot release more than you deferred through SagePay.'));
  }

  // If the transaction has expired, display an error message and redirect.
  if (time() - $transaction->created > 86400 * 30) {
    drupal_set_message(t('This authorization has passed its 90 day limit cannot be released.'), 'error');
    drupal_goto('admin/commerce/orders/' . $form_state['order']->order_id . '/payment');
  }
}

/**
 * Submit handler: Release a transaction.
 *
 * @param array $form
 *    The Form array.
 * @param array $form_state
 *   The Form state array.
 */
function commerce_sagepay_release_form_submit($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];

  commerce_sagepay_release_transaction($transaction, $amount);

  $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';

}

function commerce_sagepay_release_transaction($transaction, $amount) {

  // Set up a new transaction id for the release transaction.
  $release_transaction_id = _commerce_sagepay_vendor_tx_code($transaction);

  // Create the query to send to SagePay.
  $query = array(
    'VPSProtocol'   => SAGEPAY_PROTOCOL,
    'TxType'        => 'RELEASE',
    'Vendor'        => variable_get(SAGEPAY_SETTING_VENDOR_NAME),
    'VendorTxCode'  => $transaction->payload['VendorTxCode'],
    'ReleaseAmount' => $amount,
    'Description'   => t('Release deferred transaction.'),
    'VPSTxId'       => $transaction->remote_id,
    'SecurityKey'   => $transaction->payload['SecurityKey'],
    'TxAuthNo'      => $transaction->payload['TxAuthNo'],
  );

  // Determine the url to send the transaction to based on the transaction mode.
  switch (variable_get(SAGEPAY_SETTING_TRANSACTION_MODE)) {
    case SAGEPAY_TXN_MODE_LIVE:
      $url = SAGEPAY_SHARED_RELEASE_TRANSACTION_LIVE;
      break;

    case SAGEPAY_TXN_MODE_TEST:
      $url = SAGEPAY_SHARED_RELEASE_TRANSACTION_TEST;
      break;

    case SAGEPAY_TXN_MODE_SIMULATION:
      $url = SAGEPAY_SHARED_RELEASE_TRANSACTION_SIMULATION;
      break;
  }

  // Send the request to SagePay and process the response.
  $query = _commerce_sagepay_array_to_post($query);
  $response = _commerce_sagepay_request_post($url, $query);

  // Update and save the transaction based on the response.
  $response['VendorTxId'] = $release_transaction_id;
  $transaction->payload[REQUEST_TIME] = $response;

  switch ($response['Status']) {
    case 'OK':
      drupal_set_message(t('Payment Authorised successfully.'));
      $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
      $transaction->remote_status = SAGEPAY_REMOTE_STATUS_PAYMENT;
      break;

    default:
      // Display an error message but leave the transaction pending.
      $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
      drupal_set_message(t('Transaction release failed.'), 'error');
      drupal_set_message(check_plain($response['StatusDetail']), 'error');
  }

  $transaction_message = 'Status @status, @statusdetail. ';

  $transaction->message = $transaction_message;
  $transaction->message_variables
    = array(
    '@status'       => $response['Status'],
    '@statusdetail' => $response['StatusDetail'],
  );

  commerce_payment_transaction_save($transaction);

}
