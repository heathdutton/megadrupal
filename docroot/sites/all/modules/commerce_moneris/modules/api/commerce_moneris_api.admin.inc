<?php

/**
 * @file
 * Administrative forms for the Moneris API module.
 */

/**
 * Form callback: allows the user to refund a transaction.
 */
function commerce_moneris_api_refund_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  // If original transaction was done using HPP payment method,
  // the Refund should be done using relevant API payment method.
  if ($payment_method['method_id'] == 'commerce_moneris_hpp') {
    if (!empty($payment_method['settings']['moneris_api'])) {
      $payment_method = commerce_payment_method_instance_load($payment_method['settings']['moneris_api']);
    }
  }
  $form_state['payment_method'] = $payment_method;

  $conditions = array(
    'order_id' => $order->order_id,
    'payment_method' => $transaction->payment_method,
    'instance_id' => $transaction->instance_id,
    'remote_status' => COMMERCE_MONERIS_API_TRANSACTION_TYPE_REFUND,
    'status' => COMMERCE_PAYMENT_STATUS_SUCCESS,
  );
  $refunded_amount = 0;
  foreach (commerce_payment_transaction_load_multiple(array(), $conditions) as $capture_transaction) {
    if ($capture_transaction->payload['related_order_id'] == $transaction->payload['unique_order_id']) {
      $refunded_amount += abs($capture_transaction->amount);
    }
  }
  $remaining_amount = $transaction->amount - $refunded_amount;

  $description = implode('<br />', array(
    t('Captured: @amount', array('@amount' => commerce_currency_format($transaction->amount, $transaction->currency_code))),
    t('Refunded: @amount', array('@amount' => commerce_currency_format($refunded_amount, $transaction->currency_code))),
    t('Remaining: @amount', array('@amount' => commerce_currency_format($remaining_amount, $transaction->currency_code))),
  ));

  // Convert the price amount to a user friendly decimal value.
  $remaining_amount = number_format(commerce_currency_amount_to_decimal($remaining_amount, $transaction->currency_code), 2, '.', '');
  $form_state['max_refund_amount'] = $remaining_amount;

  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Refund amount'),
    '#description' => $description,
    '#default_value' => $remaining_amount,
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
 * Validate handler: ensure a valid amount is given.
 */
function commerce_moneris_api_refund_form_validate($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];

  // Ensure a positive numeric amount has been entered for capture.
  if (!is_numeric($amount) || $amount <= 0) {
    form_set_error('amount', t('You must specify a positive numeric amount to refund.'));
  }

  // Ensure the amount is less than or equal to the authorisation amount.
  if ($amount > $form_state['max_refund_amount']) {
    form_set_error('amount', t('You cannot refund more than you captured through Moneris.'));
  }
}

/**
 * Submit handler: process a prior authorisation capture via AIM.
 */
function commerce_moneris_api_refund_form_submit($form, &$form_state) {
  $payment_method = $form_state['payment_method'];
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];

  $response = commerce_moneris_api_transaction_refund($transaction, $amount, $payment_method);

  $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';
}

/**
 * Form callback: allows the user to capture a prior authorisation.
 */
function commerce_moneris_api_capture_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  // If original transaction was done using HPP payment method,
  // the Capture should be done using relevant API payment method.
  if ($payment_method['method_id'] == 'commerce_moneris_hpp') {
    if (!empty($payment_method['settings']['moneris_api'])) {
      $payment_method = commerce_payment_method_instance_load($payment_method['settings']['moneris_api']);
    }
  }
  $form_state['payment_method'] = $payment_method;

  $description = implode('<br />', array(
    t('Authorisation: @amount', array('@amount' => commerce_currency_format($transaction->amount, $transaction->currency_code))),
  ));

  // Convert the price amount to a user friendly decimal value.
  $amount = number_format(commerce_currency_amount_to_decimal($transaction->amount, $transaction->currency_code), 2, '.', '');

  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Capture amount'),
    '#description' => $description,
    '#default_value' => $amount,
    '#field_suffix' => check_plain($transaction->currency_code),
    '#size' => 16,
  );

  $form = confirm_form($form,
    t('What amount do you want to capture?'),
    'admin/commerce/orders/' . $order->order_id . '/payment',
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
function commerce_moneris_api_capture_form_validate($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];

  // Ensure a positive numeric amount has been entered for capture.
  if (!is_numeric($amount) || $amount <= 0) {
    form_set_error('amount', t('You must specify a positive numeric amount to capture.'));
  }

  // PreAuth may be captured for less than, equal to, or greater amount than
  // its original value - so we don't do any max amount validation here.
}

/**
 * Submit handler: process a prior authorisation capture via AIM.
 */
function commerce_moneris_api_capture_form_submit($form, &$form_state) {
  $payment_method = $form_state['payment_method'];
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];

  $response = commerce_moneris_api_transaction_capture($transaction, $amount, $payment_method);

  $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';
}

/**
 * Form callback: allows the user to void a transaction.
 */
function commerce_moneris_api_reverse_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  // If original transaction was done using HPP payment method,
  // the Reverse should be done using relevant API payment method.
  if ($payment_method['method_id'] == 'commerce_moneris_hpp') {
    if (!empty($payment_method['settings']['moneris_api'])) {
      $payment_method = commerce_payment_method_instance_load($payment_method['settings']['moneris_api']);
    }
  }
  $form_state['payment_method'] = $payment_method;

  // Convert the price amount to a user friendly decimal value.
  $form_state['amount'] = number_format(commerce_currency_amount_to_decimal($transaction->amount, $transaction->currency_code), 2, '.', '');

  $form['markup'] = array(
    '#markup' => t('Are you sure that you want to reverse this authorisation?'),
  );

  $form = confirm_form($form,
    t('Are you sure that you want to reverse this transaction?'),
    'admin/commerce/orders/' . $order->order_id . '/payment',
    '',
    t('Reverse'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit handler: process the void request.
 */
function commerce_moneris_api_reverse_form_submit($form, &$form_state) {
  $payment_method = $form_state['payment_method'];
  $transaction = $form_state['transaction'];
  $amount = $form_state['amount'];

  $response = commerce_moneris_api_transaction_reverse($transaction, $amount, $payment_method);

  $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';
}

/**
 * Form callback: allows the user to refund a transaction.
 */
function commerce_moneris_api_reauth_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  // If original transaction was done using HPP payment method,
  // the ReAuth should be done using relevant API payment method.
  if ($payment_method['method_id'] == 'commerce_moneris_hpp') {
    if (!empty($payment_method['settings']['moneris_api'])) {
      $payment_method = commerce_payment_method_instance_load($payment_method['settings']['moneris_api']);
    }
  }
  $form_state['payment_method'] = $payment_method;

  $conditions = array(
    'order_id' => $order->order_id,
    'payment_method' => 'commerce_moneris_api',
    'remote_status' => array(
      COMMERCE_MONERIS_API_TRANSACTION_TYPE_CAPTURE,
      COMMERCE_MONERIS_API_TRANSACTION_TYPE_AUTH,
    ),
    'status' => array(
      COMMERCE_PAYMENT_STATUS_SUCCESS,
      COMMERCE_PAYMENT_STATUS_MONERIS_AUTHORISED,
    ),
  );
  // If the original transaction was done using the API payment method, we need
  // to make sure that possible Reverses are done using the same instance.
  if ($transaction->payment_method == 'commerce_moneris_api') {
    $conditions['instance_id'] = $transaction->instance_id;
  }
  $captured_amount = 0;
  foreach (commerce_payment_transaction_load_multiple(array(), $conditions) as $capture_transaction) {
    if (!empty($capture_transaction->payload['related_order_id']) && $capture_transaction->payload['related_order_id'] == $transaction->payload['unique_order_id']) {
      $captured_amount += $capture_transaction->amount;
    }
  }
  $remaining_amount = $transaction->amount - $captured_amount;

  $description = implode('<br />', array(
    t('Authorised: @amount', array('@amount' => commerce_currency_format($transaction->amount, $transaction->currency_code))),
    t('Captured: @amount', array('@amount' => commerce_currency_format($captured_amount, $transaction->currency_code))),
    t('Remaining: @amount', array('@amount' => commerce_currency_format($remaining_amount, $transaction->currency_code))),
  ));

  // Convert the price amount to a user friendly decimal value.
  $remaining_amount = number_format(commerce_currency_amount_to_decimal($remaining_amount, $transaction->currency_code), 2, '.', '');
  $form_state['max_reauth_amount'] = $remaining_amount;

  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('ReAuth amount'),
    '#description' => $description,
    '#default_value' => $remaining_amount,
    '#field_suffix' => check_plain($transaction->currency_code),
    '#size' => 16,
  );

  $form = confirm_form($form,
    t('What amount do you want to re-authorise?'),
    'admin/commerce/orders/' . $order->order_id . '/payment',
    '',
    t('Re-authorise'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Validate handler: ensure a valid amount is given.
 */
function commerce_moneris_api_reauth_form_validate($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];

  // Ensure a positive numeric amount has been entered for capture.
  if (!is_numeric($amount) || $amount <= 0) {
    form_set_error('amount', t('You must specify a positive numeric amount to refund.'));
  }

  // Ensure the amount is less than or equal to the authorisation amount.
  if ($amount > $form_state['max_reauth_amount']) {
    form_set_error('amount', t('You cannot re-authorise more than the remaining uncaptured amount.'));
  }
}

/**
 * Submit handler: process a prior authorisation capture via AIM.
 */
function commerce_moneris_api_reauth_form_submit($form, &$form_state) {
  $payment_method = $form_state['payment_method'];
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];

  $response = commerce_moneris_api_transaction_reauth($transaction, $amount, $payment_method);

  $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';
}
