<?php

/**
 * @file
 * commerce_sagepay_cancel.inc
 * Handle cancel transactions.
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
 *    The Commerce Order to process.
 * @param commerce_payment_transaction $transaction
 *    The Commerce Payment Transaction to process.
 *
 * @return mixed
 *    The form array.
 */
function commerce_sagepay_cancel_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  $form_state['payment_method'] = $payment_method;

  $form = confirm_form($form,
    t('Are you sure you want to cancel this transaction?'),
    'admin/commerce/orders/' . $order->order_id . '/payment',
    '',
    t('Cancel Transaction'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}


/**
 * Submit handler.
 *
 * @param array $form
 *    The Form array.
 * @param array $form_state
 *    The Form state array.
 */
function commerce_sagepay_cancel_form_submit($form, &$form_state) {
  $transaction = $form_state['transaction'];

  commerce_sagepay_cancel_transaction($transaction);

  $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';

}


/*
 * Function to cancel an Order transaction
 *
 * @param $transaction
 *    The transaction object
 */
function commerce_sagepay_cancel_transaction($transaction) {
  // Set up the query array to send to SagePay.
  $query = array(
    'VPSProtocol'  => SAGEPAY_PROTOCOL,
    'TxType'       => 'CANCEL',
    'Vendor'       => variable_get(SAGEPAY_SETTING_VENDOR_NAME),
    'VendorTxCode' => $transaction->payload['VendorTxCode'],
    'VPSTxId'      => $transaction->remote_id,
    'SecurityKey'  => $transaction->payload['SecurityKey'],
  );

  // Determine the correct url to send the request to based on the mode.
  switch (variable_get(SAGEPAY_SETTING_TRANSACTION_MODE)) {
    case SAGEPAY_TXN_MODE_LIVE:
      $url = SAGEPAY_SHARED_CANCEL_TRANSACTION_LIVE;
      break;

    case SAGEPAY_TXN_MODE_TEST:
      $url = SAGEPAY_SHARED_CANCEL_TRANSACTION_TEST;
      break;

    case SAGEPAY_TXN_MODE_SIMULATION:
      $url = SAGEPAY_SHARED_CANCEL_TRANSACTION_SIMULATION;
      break;

  }

  // Send the request to SagePay and process the response.
  $query = _commerce_sagepay_array_to_post($query);
  $response = _commerce_sagepay_request_post($url, $query);

  // Update and save the transaction based on the response.
  $transaction->payload[REQUEST_TIME] = $response;

  switch ($response['Status']) {
    case 'OK':
      drupal_set_message(t('Payment cancelled successfully.'));

      $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
      $transaction->remote_status = SAGEPAY_REMOTE_STATUS_CANCELLED;

      // Append a capture indication to the result message.
      $transaction->message .= '<br />' . t('Cancelled: @date', array('@date' => format_date(REQUEST_TIME, 'short')));
      commerce_payment_transaction_save($transaction);

      break;

    default:
      drupal_set_message(t('Transaction Cancel failed.'), 'error');
      drupal_set_message(check_plain($response['StatusDetail']), 'error');

  }

}
