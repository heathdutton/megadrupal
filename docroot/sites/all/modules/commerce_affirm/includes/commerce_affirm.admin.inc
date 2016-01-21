<?php
/**
 * @file
 * Administrative forms for the Affirm module.
 */

/**
 * Form callback: allows the user to capture a prior authorization.
 */
function commerce_affirm_capture_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['payment_method'] = commerce_payment_method_instance_load($transaction->instance_id);
  $form_state['transaction'] = $transaction;

  $form['amount'] = array(
    '#type' => 'markup',
    '#markup' => t('Authorization: @amount', array('@amount' => commerce_currency_format($transaction->amount, $transaction->currency_code))),
  );

  $form = confirm_form($form,
    t('Are you sure you want to capture this transaction?'),
    'admin/commerce/orders/' . $order->order_id . '/payment',
    '',
    t('Capture Transaction'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit handler: process a prior authorization capture via AIM.
 */
function commerce_affirm_capture_form_submit($form, &$form_state) {
  $order = $form_state['order'];
  $payment_method = $form_state['payment_method'];
  $transaction = $form_state['transaction'];

  $data = array(
    'order_id' => $order->order_id,
  );

  $response = commerce_affirm_api_request(COMMERCE_CREDIT_CAPTURE_ONLY, $payment_method, $order, $transaction, $data);
  // Update and save the transaction based on the response.
  $transaction->payload[REQUEST_TIME . '-capture'] = $response;

  // The call is valid and the payment gateway has been approved.
  if ($response) {
    if (isset($response['status_code'])) {
      $transaction->message = '<br />' . t('Failed to capture the amount. @date', array('@date' => format_date(REQUEST_TIME, 'short')));
      drupal_set_message(t('Prior authorization capture failed, so the transaction will remain in a pending status.<br/>!message', array('!message' => $response['message'])), 'error');
    }
    else {
      // Update the transaction amount to the actual captured amount.
      // Set the remote and local status accordingly.
      $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
      $transaction->remote_status = $response['type'];
      // Append a capture indication to the result message.
      $transaction->message .= '<br />' . t('Captured: @date', array('@date' => format_date(REQUEST_TIME, 'short')));

      drupal_set_message(t('Prior authorization captured successfully.'));
    }
  }

  commerce_payment_transaction_save($transaction);

  $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';
}

/**
 * Form callback: allows the user to void a transaction.
 */
function commerce_affirm_void_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  $form_state['payment_method'] = $payment_method;

  $form['markup'] = array(
    '#markup' => t('This action cannot be undone'),
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
function commerce_affirm_void_form_submit($form, &$form_state) {
  $order = $form_state['order'];
  $payment_method = $form_state['payment_method'];
  $transaction = $form_state['transaction'];

  $response = commerce_affirm_api_request(COMMERCE_CREDIT_VOID, $payment_method, $order, $transaction);
  // Update and save the transaction based on the response.
  $transaction->payload[REQUEST_TIME . '-void'] = $response;

  // The call is valid and the payment gateway has been approved.
  if ($response) {
    if (isset($response['status_code'])) {
      $transaction->message = '<br />' . t('Failed to void the payment. @date', array('@date' => format_date(REQUEST_TIME, 'short')));
      drupal_set_message(t('Void failed'), 'error');
    }
    else {
      // Set the remote and local status accordingly.
      $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
      $transaction->remote_status = $response['type'];
      // Append a capture indication to the result message.
      $transaction->message .= '<br />' . t('Voided: @date', array('@date' => format_date(REQUEST_TIME, 'short')));

      drupal_set_message(t('Transaction successfully voided.'));
    }
  }

  commerce_payment_transaction_save($transaction);

  $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';
}

/**
 * Form callback: allows the user to issue a refund on a prior transaction.
 */
function commerce_affirm_refund_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['payment_method'] = commerce_payment_method_instance_load($transaction->instance_id);
  $form_state['transaction'] = $transaction;

  // Take the amount from the order balance to subtract it from the total.
  $balance = commerce_payment_order_balance($order);
  $default_amount = commerce_currency_amount_to_decimal($transaction->amount - $balance['amount'], $transaction->currency_code);

  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Credit amount'),
    '#description' => t('Enter the amount to be credited back to the original credit card.'),
    '#default_value' => $default_amount,
    '#field_suffix' => check_plain($transaction->currency_code),
    '#size' => 16,
  );

  $form = confirm_form($form,
    t('What amount do you want to credit?'),
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
function commerce_affirm_refund_form_validate($form, &$form_state) {
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
 * Submit handler: process a refund via Affirm.
 */
function commerce_affirm_refund_form_submit($form, &$form_state) {
  $order = $form_state['order'];
  $payment_method = $form_state['payment_method'];
  $transaction = $form_state['transaction'];

  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $order_total = $order_wrapper->commerce_order_total->value();

  $amount = commerce_currency_decimal_to_amount($form_state['values']['amount'], $order_total['currency_code']);

  $data = array(
    'amount' => $amount,
  );

  $response = commerce_affirm_api_request(COMMERCE_CREDIT_CREDIT, $payment_method, $order, $transaction, $data);

  // The call is valid and the payment gateway has been approved.
  if ($response) {
    if (isset($response['status_code'])) {
      // Update and save the transaction based on the response.
      $transaction->payload[REQUEST_TIME . '-refund'] = $response;
      $transaction->message = '<br />' . t('Failed to refund the amount. @date', array('@date' => format_date(REQUEST_TIME, 'short')));
      commerce_payment_transaction_save($transaction);

      drupal_set_message(t('Refund failed.<br/>!message', array('!message' => $response['message'])), 'error');
    }
    else {
      // Create a new transaction to record the credit.
      $credit_transaction = commerce_payment_transaction_new('affirm', $order->order_id);
      $credit_transaction->instance_id = $payment_method['instance_id'];
      $credit_transaction->remote_id = $response['id'];
      $credit_transaction->amount = $amount * -1;
      $credit_transaction->currency_code = $transaction->currency_code;
      $credit_transaction->payload[REQUEST_TIME . '-refund'] = $response;
      $credit_transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
      $credit_transaction->remote_status = $response['type'];
      $credit_transaction->message = t('Credited to @remote_id.', array('@remote_id' => $transaction->remote_id));

      // Save the credit transaction.
      commerce_payment_transaction_save($credit_transaction);

      drupal_set_message(t('Refund for @amount issued successfully', array('@amount' => commerce_currency_format($amount, $transaction->currency_code))));
    }

    $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';
  }
}
