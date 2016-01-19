<?php

/**
 * @file
 * commerce_sagepay_void.inc
 * Provides functions for voiding transactions.
 */

module_load_include('inc', 'commerce_sagepay', 'includes/commerce_sagepay_common');
module_load_include('inc', 'commerce_sagepay', 'includes/commerce_sagepay_utils');


/**
 * Form callback: allows the user to void a transaction.
 *
 * @param array $form
 *    The form array.
 * @param array $form_state
 *    The form state array
 * @param commerce_order $order
 *   The Commerce Order to process.
 * @param commerce_payment_transaction $transaction
 *   The Commerce Payment Transaction to process.
 *
 * @return mixed
 *    The form array.
 */
function commerce_sagepay_void_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  $form_state['payment_method'] = $payment_method;

  $form = confirm_form($form,
    t('Are you sure you want to void this transaction?'),
    'admin/commerce/orders/' . $order->order_id . '/payment',
    '',
    t('Void'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}


/**
 * Submit handler: Void a prior transaction.
 *
 * @param array $form
 *    The Form array.
 * @param array $form_state
 *   The Form state array.
 */
function commerce_sagepay_void_form_submit($form, &$form_state) {
  $transaction = $form_state['transaction'];

  $query = array(
    'VPSProtocol'  => SAGEPAY_PROTOCOL,
    'TxType'       => 'VOID',
    'Vendor'       => variable_get(SAGEPAY_SETTING_VENDOR_NAME),
    'VendorTxCode' => $transaction->payload['VendorTxCode'],
    'VPSTxId'      => $transaction->remote_id,
    'SecurityKey'  => $transaction->payload['SecurityKey'],
    'TxAuthNo'     => $transaction->payload['TxAuthNo'],
  );

  switch (variable_get(SAGEPAY_SETTING_TRANSACTION_MODE)) {
    case SAGEPAY_TXN_MODE_LIVE:
      $url = SAGEPAY_SHARED_VOID_TRANSACTION_LIVE;
      break;

    case SAGEPAY_TXN_MODE_TEST:
      $url = SAGEPAY_SHARED_VOID_TRANSACTION_TEST;
      break;

    case SAGEPAY_TXN_MODE_SIMULATION:
      $url = SAGEPAY_SHARED_VOID_TRANSACTION_SIMULATION;
      break;

  }

  $query = _commerce_sagepay_array_to_post($query);

  $response = _commerce_sagepay_request_post($url, $query);

  // Update and save the transaction based on the response.
  $transaction->payload[REQUEST_TIME] = $response;

  switch ($response['Status']) {
    case 'OK':
      drupal_set_message(t('Payment voided successfully.'));
      // Update the transaction amount to the actual capture amount.
      $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
      $transaction->remote_status = SAGEPAY_REMOTE_STATUS_CANCELLED;

      // Append a capture indication to the result message.
      $transaction->message .= '<br />' . t('Aborted: @date', array('@date' => format_date(REQUEST_TIME, 'short')));
      commerce_payment_transaction_save($transaction);

      break;

    default:
      // Display an error message but leave the transaction pending.
      drupal_set_message(t('Transaction Void failed.'), 'error');
      drupal_set_message(check_plain($response['StatusDetail']), 'error');

  }
  $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';

}
