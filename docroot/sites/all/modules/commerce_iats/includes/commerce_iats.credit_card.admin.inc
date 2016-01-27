<?php

/**
 * @file
 * Administrative forms for the credit card processing functionality
 * of the Commerce iATS module.
 */

/**
 * Form callback: allows the user to issue a refund on a prior transaction.
 */
function commerce_iats_credit_card_refund_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  $form_state['payment_method'] = $payment_method;

  $default_amount = number_format(
    commerce_currency_amount_to_decimal($transaction->amount,
      $transaction->currency_code), 2, '.', '');

  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Refund amount'),
    '#description' => t('Enter the amount to be refunded back to the original card.'),
    '#default_value' => $default_amount,
    '#field_suffix' => check_plain($transaction->currency_code),
    '#size' => 16,
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
 * Validate handler: check the refund amount before attempting refund request.
 */
function commerce_iats_credit_card_refund_form_validate($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];

  // Ensure a positive numeric amount has been entered for refund.
  if (!is_numeric($amount) || $amount <= 0) {
    form_set_error('amount', t('You must specify a positive numeric amount to refund.'));
  }

  // Ensure the amount is less than or equal to the captured amount.
  if ($amount > commerce_currency_amount_to_decimal($transaction->amount, $transaction->currency_code)) {
    form_set_error('amount', t('You cannot refund more than you captured through iATS.'));
  }

  // If the transaction is older than 120 days, display an error message and
  // redirect.
  if (time() - $transaction->created > 86400 * 120) {
    drupal_set_message(t('This capture has passed its 120 day limit for issuing refunds.'), 'error');
    drupal_goto('admin/commerce/orders/' . $form_state['order']->order_id . '/payment');
  }
}

/**
 * Submit handler: process a credit card refund.
 */
function commerce_iats_credit_card_refund_form_submit($form, &$form_state) {
  $payment_data = array(
    'transaction' => $form_state['transaction'],
  );

  $amount = number_format($form_state['values']['amount'], 2, '.', '');

  $charge = array(
    // Charge must be negative for refunds.
    'amount' => ($amount * -1),
    'currency_code' => $payment_data['transaction']->currency_code,
  );

  commerce_iats_process_payment($form_state['payment_method'], $payment_data,
    $form_state['order'], $charge, 'commerce_iats_process_credit_card_refund');

  $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';
}

/**
 * Processes a credit card refund via the iATS Payments API.
 *
 * @param array $payment_method
 *   The payment method used during this transaction.
 * @param array $payment_data
 *   Payment data retrieved from the submit form.
 * @param object $order
 *   The order processed during this transaction.
 * @param array $charge
 *   The transaction charge. Passed by reference here as refunds need to
 *   modify the charge amount.
 *
 * @return Mixed
 *   API response array or error string.
 */
function commerce_iats_process_credit_card_refund($payment_method, $payment_data, $order, &$charge) {
  // Get the original transaction ID.
  $remote_id_parts = explode('|', $payment_data['transaction']->remote_id);

  // Create and populate the API request.
  $request = array(
    'customerIPAddress' => ip_address(),
    'transactionId' => $remote_id_parts[0],
    'total' => $charge['amount'],
    'comment' => COMMERCE_IATS_COMMENT_PREFIX . t('Refund on order') . ' ' . $order->order_number,
  );

  // Get the API service.
  $iats = commerce_iats_get_service('ProcessLink', $payment_method);
  if ($iats == NULL) {
    drupal_set_message(t('Unable to load iATS Payments library.'), 'error');
    return NULL;
  }

  // Log the request.
  if ($payment_method['settings']['log']['request'] === 'request') {
    commerce_iats_log_request($payment_method, 'commerce_iats_process_credit_card_refund', $request);
  }

  // Process the credit card refund.
  $response = $iats->processCreditCardRefundWithTransactionId($request);

  // Currency not automatically converted to DB-friendly amount when
  // processing refund. Do it here manually.
  $charge['amount'] = commerce_currency_decimal_to_amount($charge['amount'],
    $payment_data['transaction']->currency_code);

  return $response;
}
