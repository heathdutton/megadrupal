<?php

/**
 * @file
 * Administrative forms for the Commerce ETS module.
 */

/**
 * Form callback: Void a prior transaction.
 */
function commerce_ets_emoney_void_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  $form_state['payment_method'] = $payment_method;

  $form['markup'] = array(
    '#markup' => t('Are you sure that you want to void this transaction?'),
  );

  $form = confirm_form($form,
    t('Are you sure that you want to void this transaction?'),
    'admin/commerce/orders/' . $order->order_id . '/payment',
    '',
    t('Void'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit handler: process a void via ETS Emoney.
 */
function commerce_ets_emoney_void_form_submit($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $order = $form_state['order'];
  $payment_method = $form_state['payment_method'];

  if ($library = commerce_ets_emoney_get_library($payment_method)) {
    $session = $library->set('action', 'void')
      ->set('sessionID', $transaction->data['session_id'])
      ->set('transactionID', $transaction->remote_id)
      ->send();
    $transaction->payload[REQUEST_TIME] = (array) $session->transactions;

    if ($session->status === 'success') {
      drupal_set_message(t('Transaction successfully voided.'));
      $transaction->remote_status = $session->transactions->status;
      $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
      $transaction->message .= '<br />' . t('Voided: @date', array('@date' => format_date(REQUEST_TIME, 'short')));
    }
    else {
      $reason = !empty($session->message) ? $session->message : '';
      drupal_set_message(t('Void failed: @reason', array('@reason' => $reason)), 'error');
    }

    commerce_payment_transaction_save($transaction);
  }
  else {
    drupal_set_message(t('The payment library could not be loaded'), 'error');
  }

  $form_state['redirect'] = 'admin/commerce/orders/' . $order->order_id . '/payment';
}

/**
 * Form callback: allows the user to issue a refund on a prior transaction.
 */
function commerce_ets_emoney_refund_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  $form_state['payment_method'] = $payment_method;

  $default_amount = number_format(commerce_currency_amount_to_decimal($transaction->amount, $transaction->currency_code), 2, '.', '');

  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Credit amount'),
    '#description' => t('Enter the amount to be credited back to the original credit card.'),
    '#default_value' => $default_amount,
    '#field_suffix' => check_plain($transaction->currency_code),
    '#size' => 16,
  );

  $form = confirm_form($form,
    t('What amount do you want to refund?'),
    'admin/commerce/orders/' . $order->order_id . '/payment',
    '',
    t('Credit'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Validate handler: check the refund amount before attempting refund request.
 */
function commerce_ets_emoney_refund_form_validate($form, &$form_state) {
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
}

/**
 * Submit handler: process a refund via ETS Emoney.
 */
function commerce_ets_emoney_refund_form_submit($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = number_format($form_state['values']['amount'], 2, '.', '');
  $order = $form_state['order'];
  $payment_method = $form_state['payment_method'];

  if ($library = commerce_ets_emoney_get_library($payment_method)) {
    $session = $library->set('action', 'refund')
      ->set('transactionID', $transaction->remote_id)
      ->send();

    if ($session->status === 'success') {
      $refund_amount = commerce_currency_decimal_to_amount($amount, $transaction->currency_code);
      drupal_set_message(t('Refund for @amount issued successfully', array('@amount' => commerce_currency_format($refund_amount, $transaction->currency_code))));

      // Create a new transaction to record the refund.
      $refund_transaction = commerce_payment_transaction_new('ets_emoney', $order->order_id);
      $refund_transaction->instance_id = $payment_method['instance_id'];
      $refund_transaction->remote_id = $session->transactions->id;
      $refund_transaction->amount = $refund_amount * -1;
      $refund_transaction->currency_code = $transaction->currency_code;
      $refund_transaction->payload[REQUEST_TIME] = (array) $session->transactions;
      $refund_transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
      $refund_transaction->remote_status = $session->transactions->status;
      $refund_transaction->message = t('Refunded to @remote_id.', array('@remote_id' => $transaction->remote_id));

      // Save the refund transaction.
      commerce_payment_transaction_save($refund_transaction);
    }
    else {
      $transaction->payload[REQUEST_TIME] = (array) $session->transactions;
      $reason = !empty($session->message) ? $session->message : '';

      drupal_set_message(t('Refund failed: @reason', array('@reason' => $reason), 'error'));

      commerce_payment_transaction_save($transaction);
    }
  }
  else {
    drupal_set_message(t('The payment library could not be loaded'), 'error');
  }

  $form_state['redirect'] = 'admin/commerce/orders/' . $order->order_id . '/payment';
}
