<?php

/**
 * @file
 * Settings forms for the Commerce Be2Bill Payment Gateway module.
 */

/**
 * Implements hook_form().
 *
 * Allows the user to capture a previous authorization.
 */
function commerce_be2bill_capture_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  $form_state['payment_method'] = $payment_method;

  $balance = commerce_payment_order_balance($order);

  if ($balance['amount'] > 0 && $balance['amount'] < $transaction->amount) {
    $default_amount = $balance['amount'];
  }
  else {
    $default_amount = $transaction->amount;
  }

  // Convert the price amount to a user friendly decimal value.
  $default_amount = commerce_currency_amount_to_decimal($default_amount, $transaction->currency_code);

  $description = implode('<br />', array(
                           t('Authorization: @amount', array('@amount' => commerce_currency_format($transaction->amount, $transaction->currency_code))),
                           t('Order balance: @balance', array('@balance' => commerce_currency_format($balance['amount'], $balance['currency_code']))),
                         ));

  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Capture amount'),
    '#description' => $description,
    '#default_value' => $default_amount,
    '#field_suffix' => check_plain($transaction->currency_code),
    '#size' => 16,
  );

  $form = confirm_form($form,
                       t('What amount do you want to capture?'),
                       "admin/commerce/orders/$order->order_id/payment",
                       '',
                       t('Capture'),
                       t('Cancel'),
                       'confirm'
  );

  return $form;
}

/**
 * Validate handler: ensure a valid amount is given.
 */
function commerce_be2bill_capture_form_validate($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];

  // Ensure a positive numeric amount has been entered for capture.
  if (!is_numeric($amount) || $amount <= 0) {
    form_set_error('amount', t('You must specify a positive numeric amount to capture.'));
  }

  // Ensure the amount is less than or equal to the authorization amount.
  if ($amount > commerce_currency_amount_to_decimal($transaction->amount, $transaction->currency_code)) {
    form_set_error('amount', t('You cannot capture more than you authorized through Be2Bill.'));
  }

  // If the authorization has expired, display an error message and redirect.
  if (REQUEST_TIME - $transaction->created > 3600 * 24 * COMMERCE_BE2BILL_AUTHORIZATION_MAX_DELAY_DAYS) {
    drupal_set_message(
      t('This authorization has passed its @max_days day limit cannot be captured.',
        array('@max_days' => COMMERCE_BE2BILL_AUTHORIZATION_MAX_DELAY_DAYS)
      ),
      'error');
    drupal_goto('admin/commerce/orders/' . $form_state['order']->order_id . '/payment');
  }
}

/**
 * Submit handler: process a prior authorization capture.
 */
function commerce_be2bill_capture_form_submit($form, &$form_state) {

  $transaction = $form_state['transaction'];
  $amount = commerce_currency_decimal_to_amount($form_state['values']['amount'], $transaction->currency_code);
  $order = $form_state['order'];

  // Because the IPN request sent in synchronous and this form needs to load the
  // order at the same time we're obliged to release the lock to avoid a
  // timeout error.
  entity_get_controller('commerce_order')->resetCache(array($order->order_id));

  // Load and store the payment method instance for this transaction.
  $current_settings = commerce_payment_method_instance_load($transaction->instance_id);
  // Get the current payment method settings.
  $current_settings = $current_settings['settings'];

  // Build the request to Be2Bill.
  $options = array(
    'method' => 'POST',
    'data' => http_build_query(
      array(
        'method' => 'capture',
        'params' => commerce_be2bill_request_capture($current_settings, $order, $transaction, $amount),
      )
    ),
    'headers' => array('Content-Type' => 'application/x-www-form-urlencoded; charset=utf-8'),
    // Get the SSL context.
    'context' => _commerce_be2bill_ssl_context(
      TRUE,
      COMMERCE_BE2BILL_SSL_PEER_VALIDATE,
      COMMERCE_BE2BILL_SSL_CA_FILE
    ),
  );

  $result = drupal_http_request(
    commerce_be2bill_server_url($current_settings['txn_mode'], 'server'),
    $options);
  $response = drupal_json_decode($result->data);

  // Update and save the transaction based on the response.
  $transaction->payload[REQUEST_TIME . '-capture'] = $response;

  // In the case of an unsuccessful transaction.
  if ($response['EXECCODE'] != COMMERCE_BE2BILL_TRANSACTION_ACCEPTED
      &&
      $response['EXECCODE'] != COMMERCE_BE2BILL_ENROLLED_CARD_ID_NEEDED) {
    // Display an error message but leave the transaction pending.
    drupal_set_message(t('Prior authorization capture failed, so the transaction will remain in a pending status.'), 'error');
    drupal_set_message(check_plain($response['MESSAGE']), 'error');
  }
  else {
    drupal_set_message(t('Prior authorization captured successfully.'));

    // Update the transaction amount to the actual capture amount.
    $transaction->amount = $amount;

    // Set the remote and local status accordingly.
    $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
    $transaction->remote_status = $response['OPERATIONTYPE'];;
    $transaction->remote_id = $response['TRANSACTIONID'];

    // Append a capture notice to the result message.
    $transaction->message .= '<br />' . t('Captured: @date', array('@date' => format_date(REQUEST_TIME, 'short')));
  }
  // Save the transaction.
  commerce_payment_transaction_save($transaction);
  $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';
}

/**
 * Implements hook_form().
 *
 * Form callback for voiding or refunding a previous transaction.
 *
 */
function commerce_be2bill_refund_void_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  // Load and store the payment method instance for this transaction.
  $form_state['payment_method'] = commerce_payment_method_instance_load($transaction->instance_id);;

  $refund_amount = commerce_be2bill_get_refund_amount($order, $transaction);

  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Refund amount'),
    '#description' => t('Enter the amount to be refunded back to the customer. Maximum amount to refund: <strong>@refund @currency</strong>.', array('@refund' => $refund_amount, '@currency' => $transaction->currency_code)),
    '#default_value' => $refund_amount,
    '#field_suffix' => check_plain($transaction->currency_code),
    '#size' => 16,
  );

  return confirm_form($form,
                      t('What amount do you want to refund?'),
                      "admin/commerce/orders/$order->order_id/payment",
                      '',
                      t('Refund'),
                      t('Cancel'),
                      'confirm');
}

/**
 * Validate handler: check the amount before attempting a refund request.
 */
function commerce_be2bill_refund_void_form_validate($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];

  // Ensure a positive numeric amount has been entered for refund.
  if (!is_numeric($amount) || $amount <= 0) {
    form_set_error('amount', t('You must specify a positive numeric amount to refund.'));
  }

  // Ensure the amount is less than or equal to the captured amount.
  if ($amount > commerce_currency_amount_to_decimal($transaction->amount, $transaction->currency_code)) {
    form_set_error('amount', t('You cannot refund more than you captured.'));
  }

  // If the transaction is older than the maximum delay display an error message and redirect.
  if (REQUEST_TIME - $transaction->created > 3600 * 24 * COMMERCE_BE2BILL_REFUND_MAX_DELAY_DAYS) {
    drupal_set_message(
      t('This transaction has passed its @max_days day limit for refunding.',
        array('@max_days' => COMMERCE_BE2BILL_REFUND_MAX_DELAY_DAYS)
      ),
      'error');
    drupal_goto('admin/commerce/orders/' . $form_state['order']->order_id . '/payment');
  }
}

/**
 * Submit handler for processing a refund request.
 */
function commerce_be2bill_refund_void_form_submit($form, &$form_state) {
  // Load the needed entities.
  $transaction = $form_state['transaction'];
  $payment_method = $form_state['payment_method'];
  $order = $form_state['order'];
  // Get the refund values.
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $order_total = $order_wrapper->commerce_order_total->value();
  $amount = commerce_currency_decimal_to_amount($form_state['values']['amount'], $order_total['currency_code']);
  // Build the request to Be2Bill.
  $options = array(
    'method' => 'POST',
    'data' => http_build_query(
      array(
        'method' => 'refund',
        'params' => commerce_be2bill_request_refund_void($payment_method['settings'], $order, $transaction, $amount),
      )
    ),
    'headers' => array('Content-Type' => 'application/x-www-form-urlencoded; charset=utf-8'),
    // Get the SSL context.
    'context' => _commerce_be2bill_ssl_context(
      TRUE,
      COMMERCE_BE2BILL_SSL_PEER_VALIDATE,
      COMMERCE_BE2BILL_SSL_CA_FILE
    ),
  );

  $result = drupal_http_request(
    commerce_be2bill_server_url($payment_method['settings']['txn_mode'], 'server'),
    $options);
  $response = isset($result->data) ? drupal_json_decode($result->data) : NULL;

  if (!empty($response)) {
    $transaction->payload[REQUEST_TIME . '-refund'] = $response;

    // If we didn't get a refund approval response code.
    if ($response['EXECCODE'] != COMMERCE_BE2BILL_TRANSACTION_ACCEPTED
        &&
        $response['EXECCODE'] != COMMERCE_BE2BILL_ENROLLED_CARD_ID_NEEDED) {
      // Display a tentatively informative error message based on the response
      // returned by Be2Bill.
      drupal_set_message(t('The refund operation failed with: @reason',
                           array('@reason' => $response['MESSAGE'])), 'error');
    }
    else {
      // Void the transaction if the refunded amount is equal to the transaction
      // amount.
      if ($amount == $transaction->amount) {
        drupal_set_message(t('Transaction successfully voided.'));
        // Set the remote and local status accordingly.

        // For refunding we create specific status. Refunding is not a capture
        // or an authorization.
        $transaction->status = COMMERCE_BE2BILL_PAYMENT_STATUS_REFUND_FAILS;
        $transaction->remote_status = $response['OPERATIONTYPE'];
        $transaction->remote_id = $response['TRANSACTIONID'];
        // Append a capture indication to the result message.
        $transaction->message .= '<br />' . t('Voided: @date', array('@date' => format_date(REQUEST_TIME, 'short')));
      }
      // Do a partial refund.
      else {
        drupal_set_message(t('Refund for @amount issued successfully.', array('@amount' => commerce_currency_format($amount, $transaction->currency_code))));
        // Create a new transaction to record the credit.
        $credit_transaction = commerce_payment_transaction_new($payment_method['method_id'], $order->order_id);
        $credit_transaction->instance_id = $payment_method['instance_id'];
        $credit_transaction->remote_id = $response['TRANSACTIONID'];
        $credit_transaction->amount = $amount;
        $credit_transaction->currency_code = $transaction->currency_code;
        $credit_transaction->payload[REQUEST_TIME] = $response;
        $credit_transaction->status = COMMERCE_BE2BILL_PAYMENT_STATUS_REFUND_OK;
        $credit_transaction->remote_status = $response['OPERATIONTYPE'];
        $credit_transaction->message = t('Refunded payment for Order @order_id.',
                                         array('@order_id' => $order->order_id));

        // Save the credit transaction.
        commerce_payment_transaction_save($credit_transaction);
      }
      commerce_payment_transaction_save($transaction);}
  }
  else {
    drupal_set_message(t('The refund operation failed'));
  }

  $form_state['redirect'] = "admin/commerce/orders/$order->order_id/payment";
}
