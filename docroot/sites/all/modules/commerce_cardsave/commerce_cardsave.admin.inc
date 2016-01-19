<?php

/**
 * @file
 * Admin forms for Commerce Cardsave.
 */

/**
 * Form callback: allows the user to capture a prior authorization.
 */
function commerce_cardsave_capture_form($form, &$form_state, $order, $transaction) {
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
  $default_amount = number_format(commerce_currency_amount_to_decimal($default_amount, $transaction->currency_code), 2, '.', '');

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
function commerce_cardsave_capture_form_validate($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];

  // Ensure a positive numeric amount has been entered for capture.
  if (!is_numeric($amount) || $amount <= 0) {
    form_set_error('amount', t('You must specify a positive numeric amount to capture.'));
  }

  // Ensure the amount is less than or equal to the authorization amount.
  if ($amount > commerce_currency_amount_to_decimal($transaction->amount, $transaction->currency_code)) {
    form_set_error('amount', t('You cannot capture more than you authorized through CardSave.'));
  }

  // If the authorization has expired, display an error message and redirect.
  if (REQUEST_TIME - $transaction->created > 86400 * 3) {
    drupal_set_message(t('This authorization has passed its 3 day limit cannot be captured.'), 'error');
    drupal_goto('admin/commerce/orders/' . $form_state['order']->order_id . '/payment');
  }
}

/**
 * Submit handler: process a prior authorization capture via CardSave.
 */
function commerce_cardsave_capture_form_submit($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $order = commerce_order_load($transaction->order_id);
  $amount = commerce_currency_decimal_to_amount($form_state['values']['amount'], $transaction->currency_code);
  $description = t('Authorization capture for order #@order_id at @site_name', array(
    '@order_id' => $order->order_id,
    '@site_name' => variable_get('site_name', ''),
  ));
  $api = commerce_cardsave_get_api();

  $api->crossPayment($order->order_id, $amount, $transaction->currency_code, $description, $transaction->remote_id, 'COLLECTION', 'FALSE');

  if ($api->getSuccess()) {
    drupal_set_message(t('Prior authorization captured successfully.'));

    $transaction->amount = $amount;
    $transaction->remote_id = $api->getCrossReference();
    $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
    $transaction->message .= '<br />' . t('Captured: @date', array('@date' => format_date(REQUEST_TIME, 'short')));
  }
  else {
    drupal_set_message(t('Prior authorization capture failed, so the transaction will remain in a pending status.'), 'error');
    drupal_set_message(check_plain($api->getMessage()), 'error');
  }

  commerce_payment_transaction_save($transaction);

  $form_state['redirect'] = 'admin/commerce/orders/' . $order->order_id . '/payment';
}

/**
 * Form callback: allows the user to refund a prior capture.
 */
function commerce_cardsave_refund_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  $form_state['payment_method'] = $payment_method;

  $default_amount = $transaction->amount;

  // Convert the price amount to a user friendly decimal value.
  $default_amount = commerce_currency_amount_to_decimal($default_amount, $transaction->currency_code);

  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Refund amount'),
    '#description' => t('Captured: @amount', array('@amount' => commerce_currency_format($transaction->amount, $transaction->currency_code))),
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
 * Validate handler: ensure a valid amount is given.
 */
function commerce_cardsave_refund_form_validate($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];

  // Ensure a positive numeric amount has been entered for capture.
  if (!is_numeric($amount) || $amount <= 0) {
    form_set_error('amount', t('You must specify a positive numeric amount to refund.'));
  }

  // Ensure the amount is less than or equal to the authorization amount.
  if ($amount > commerce_currency_amount_to_decimal($transaction->amount, $transaction->currency_code)) {
    form_set_error('amount', t('You cannot refund more than you captured through CardSave.'));
  }

  // If the authorization has expired, display an error message and redirect.
  if (REQUEST_TIME - $transaction->created > 86400 * 30) {
    drupal_set_message(t('This authorization has passed its 30 day limit cannot be refunded.'), 'error');
    drupal_goto('admin/commerce/orders/' . $form_state['order']->order_id . '/payment');
  }
}

/**
 * Submit handler: process a prior authorization capture via AIM.
 */
function commerce_cardsave_refund_form_submit($form, &$form_state) {
  $prev_transaction = $form_state['transaction'];
  $amount = commerce_currency_decimal_to_amount($form_state['values']['amount'], $prev_transaction->currency_code);
  $payment_method = commerce_payment_method_instance_load($prev_transaction->instance_id);
  $order = commerce_order_load($prev_transaction->order_id);
  $description = t('Refund of order #@order_id at @site_name', array(
    '@order_id' => $order->order_id,
    '@site_name' => variable_get('site_name', ''),
  ));
  $api = commerce_cardsave_get_api();

  $api->crossPayment($order->order_id, $amount, $prev_transaction->currency_code, $description, $prev_transaction->remote_id, 'REFUND');

  if ($api->getSuccess()) {
    $transaction = commerce_payment_transaction_new('cardsave', $prev_transaction->order_id);
    $transaction->amount = -$amount;
    $transaction->currency_code = $prev_transaction->currency_code;
    $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
    $transaction->message = t('Refund transaction');
    $transaction->instance_id = $payment_method['instance_id'];
    $transaction->remote_id = $api->getCrossReference();

    commerce_payment_transaction_save($transaction);
  }
  else {
    drupal_set_message(check_plain($api->getMessage()), 'error');
  }

  $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';
}

/**
 * Form callback: allows the user to void a transaction.
 */
function commerce_cardsave_void_form($form, &$form_state, $order, $transaction) {
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
 * Submit handler: process the void request.
 */
function commerce_cardsave_void_form_submit($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $order = commerce_order_load($transaction->order_id);
  $description = t('Voided transaction');
  $api = commerce_cardsave_get_api();

  $api->crossPayment($order->order_id, $transaction->amount, $transaction->currency_code, $description, $transaction->remote_id, 'VOID', 'FALSE');

  if ($api->getSuccess()) {
    $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
    $transaction->message .= '<br />' . $description;
    $transaction->remote_id = $api->getCrossReference();

    commerce_payment_transaction_save($transaction);
  }
  else {
    drupal_set_message(check_plain($api->getMessage()), 'error');
  }

  $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';
}
