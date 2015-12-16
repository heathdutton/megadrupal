<?php

/**
 * @file
 * commerce_sagepay_authorise.inc
 * Handles payment authorisations.
 */

module_load_include('inc', 'commerce_sagepay', 'includes/commerce_sagepay_common');
module_load_include('inc', 'commerce_sagepay', 'includes/commerce_sagepay_utils');

/**
 * Form callback: allows the user to Authorise a transaction.
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
function commerce_sagepay_authorise_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  $form_state['payment_method'] = $payment_method;

  $balance = commerce_payment_order_balance($order);

  // Assume we are going to want to Authorise the whole transaction.
  $default_amount = $transaction->amount;

  // Convert the price amount to a user friendly decimal value.
  $default_amount = commerce_currency_amount_to_decimal($default_amount, $transaction->currency_code);

  // Render a form so the user can decide how much to authorise.
  $description = implode('<br />', array(
    t('Authorise: @amount', array('@amount' => commerce_currency_format($transaction->amount, $transaction->currency_code))),
    t('Order balance: @balance', array('@balance' => commerce_currency_format($balance['amount'], $balance['currency_code']))),
  ));

  $form['amount'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Authorise amount'),
    '#description'   => check_plain($description),
    '#default_value' => $default_amount,
    '#field_suffix'  => check_plain($transaction->currency_code),
    '#size'          => 16,
  );

  $form['cv2_check'] = array(
    '#type'    => 'select',
    '#title'   => 'Override AVS / CV2 check for this transaction',
    '#options' => array(
      '0' => 'If AVS/CV2 enabled then check them. If rules apply, use rules. (default)',
      '1' => 'Force AVS/CV2 checks even if not enabled for the account. If rules apply, use rules.',
      '2' => 'Force NO AVS/CV2 checks even if enabled on account. 3 = Force AVS/CV2 checks even if not enabled for the account but DON’T apply any rules.',
    ),
  );

  $form = confirm_form($form,
    t('How much do you want to authorise?'),
    'admin/commerce/orders/' . $order->order_id . '/payment',
    '',
    t('Authorise'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Validate handler: ensure a valid amount is given.
 *
 * @param array $form
 *    The form array.
 * @param array $form_state
 *    The form state array.
 */
function commerce_sagepay_authorise_form_validate($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];

  // Ensure a positive numeric amount has been entered for capture.
  if (!is_numeric($amount) || $amount <= 0) {
    form_set_error('amount', t('You must specify a positive numeric amount to Authorise.'));
  }

  // Ensure the amount is less than or equal to the transaction amount.
  if ($amount > commerce_currency_amount_to_decimal($transaction->amount, $transaction->currency_code)) {
    form_set_error('amount', t('You cannot Authorise more than you collected through SagePay.'));
  }

  // If the authorization has expired, display an error message and redirect.
  if (time() - $transaction->created > 86400 * 30) {
    drupal_set_message(t('This authorization has passed its 90 day limit cannot be authorised.'), 'error');
    drupal_goto('admin/commerce/orders/' . $form_state['order']->order_id . '/payment');
  }
}

/**
 * Submit handler: process a prior authorization capture.
 *
 * @param array $form
 *    The Form array.
 * @param array $form_state
 *    The Form state array.
 */
function commerce_sagepay_authorise_form_submit($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];

  // Create a new transaction number for the authorisation transaction.
  $authorise_transaction_id = _commerce_sagepay_vendor_tx_code($transaction);

  // Assemble the query array to send to SagePay.
  $query = array(
    'VPSProtocol'         => SAGEPAY_PROTOCOL,
    'TxType'              => 'AUTHORISE',
    'Vendor'              => variable_get(SAGEPAY_SETTING_VENDOR_NAME),
    'VendorTxCode'        => $authorise_transaction_id,
    'Amount'              => $amount,
    'Description'         => t('Authorise registered transaction.'),
    'RelatedVPSTxId'      => $transaction->remote_id,
    'RelatedVendorTxCode' => $transaction->payload['VendorTxCode'],
    'RelatedSecurityKey'  => $transaction->payload['SecurityKey'],
    'ApplyAVSCV2'         => $form_state['values']['cv2_check'],
  );

  // Determine the correct url to send the request depending on the Mode.
  switch (variable_get(SAGEPAY_SETTING_TRANSACTION_MODE)) {
    case SAGEPAY_TXN_MODE_LIVE:
      $url = SAGEPAY_SHARED_AUTHORISE_TRANSACTION_LIVE;
      break;

    case SAGEPAY_TXN_MODE_TEST:
      $url = SAGEPAY_SHARED_AUTHORISE_TRANSACTION_TEST;
      break;

    case SAGEPAY_TXN_MODE_SIMULATION:
      $url = SAGEPAY_SHARED_AUTHORISE_TRANSACTION_SIMULATION;
      break;
  }

  // Send the request to SagePay adn process the response.
  $query = _commerce_sagepay_array_to_post($query);
  $response = _commerce_sagepay_request_post($url, $query);

  // Update and save the transaction based on the response.
  $response['VendorTxId'] = $authorise_transaction_id;
  $transaction->payload[REQUEST_TIME] = $response;
  $transaction->payload['VendorTxId'] = $authorise_transaction_id;

  // Create a new transaction for the Authorise.
  $authorise_transaction = commerce_payment_transaction_new($transaction->payment_method, $transaction->order_id);
  $authorise_transaction->instance_id = $transaction->instance_id;
  $authorise_transaction->amount = $amount * 100;
  $authorise_transaction->currency_code = $transaction->currency_code;
  $authorise_transaction->remote_id = $authorise_transaction_id;

  $authorise_transaction->payload = $response;

  switch ($response['Status']) {
    case 'OK':
      drupal_set_message(t('Payment Authorised successfully.'));
      $authorise_transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
      break;

    default:
      // Display an error message but leave the transaction pending.
      $authorise_transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
      drupal_set_message(t('Transaction authorise failed.'), 'error');
      drupal_set_message(check_plain($response['StatusDetail']), 'error');
  }

  $transaction_message = 'Status @status, @statusdetail. ';

  $authorise_transaction->message = $transaction_message;
  $authorise_transaction->message_variables
    = array(
      '@status'       => $response['Status'],
      '@statusdetail' => $response['StatusDetail'],
    );

  commerce_payment_transaction_save($authorise_transaction);

  $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';
}
