<?php

/**
 * @file
 * Additional administrative forms and functions for First Data
 */


/**
 * Form callback: allows the user to capture a prior authorization.
 */
function commerce_firstdata_cc_capture_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  $form_state['payment_method'] = $payment_method;

  $balance = commerce_payment_order_balance($order);

  // If the balance is below the transaction amount, use that amount for the preauth instead
  if ($balance['amount'] > 0 && $balance['amount'] < $transaction->amount) {
    $default_amount = $balance['amount'];
  }
  // Otherwise just use the original transaction amount
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
function commerce_firstdata_cc_capture_form_validate($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];

  // Ensure a positive numeric amount has been entered for capture.
  if (!is_numeric($amount) || $amount <= 0) {
    form_set_error('amount', t('You must specify a positive numeric amount to capture.'));
  }

  // Ensure the amount is less than or equal to the authorization amount.
  if ($amount > commerce_currency_amount_to_decimal($transaction->amount, $transaction->currency_code)) {
    form_set_error('amount', t('You cannot capture more than you authorized through First Data.'));
  }

  // If the authorization has expired, display an error message and redirect.
  if (time() - $transaction->created > 86400 * 30) {
    drupal_set_message(t('This authorization has passed its 30 day limit cannot be captured.'), 'error');
    drupal_goto('admin/commerce/orders/' . $form_state['order']->order_id . '/payment');
  }
}

/**
 * Submit handler: process a prior authorization capture via AIM.
 */
function commerce_firstdata_cc_capture_form_submit($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];
  $payment_method = $form_state['payment_method'];
  $order = $form_state['order'];

  // build transaction XML
  $transaction_xml = '<order>';
  $transaction_xml .= commerce_firstdata_xml_merchantinfo($payment_method);
  $transaction_xml .= commerce_firstdata_xml_transcationdetails($order, $transaction->remote_id);
  $transaction_xml .= commerce_firstdata_xml_orderoptions($payment_method, COMMERCE_CREDIT_PRIOR_AUTH_CAPTURE);
  $transaction_xml .= commerce_firstdata_xml_payment($amount);
  $transaction_xml .= '</order>';
  $transaction_xml = commerce_firstdata_clean_xml($transaction_xml);

  // Send the transcation to First Data
  $transaction_result = firstdata_commerce_send_transaction($transaction_xml, $payment_method);

  // Update and save the transaction based on the response.
  $transaction->payload[REQUEST_TIME] = $transaction_result['payload'];

  // If we didn't get an approval response code...
  if ($transaction_result['approved'] != 'APPROVED') {
    // Display an error message but leave the transaction pending.
    drupal_set_message(t('Prior authorization capture failed, so the transaction will remain in a pending status.'), 'error');
    drupal_set_message(check_plain($transaction_result['error']), 'error');
  }
  else {
    drupal_set_message(t('Prior authorization captured successfully.'));

    // Update the transaction amount to the actual capture amount.
    $transaction->amount = commerce_currency_decimal_to_amount($amount, $transaction->currency_code);

    // Set the remote and local status accordingly.
    $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
    $transaction->remote_status = commerce_firstdata_transaction_map(COMMERCE_CREDIT_PRIOR_AUTH_CAPTURE);

    // Append a capture indication to the result message.
    $transaction->message .= '<br />' . t('<b>Funds Captured</b>: @date', array('@date' => format_date(REQUEST_TIME, 'short')));
  }

  commerce_payment_transaction_save($transaction);

  $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';
}

/**
 * Form callback: allows the user to void a transcation.
 */
function commerce_firstdata_cc_void_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  $form_state['payment_method'] = $payment_method;

  // figure out how much is being voided
  $void_amount = commerce_currency_amount_to_decimal($transaction->amount, $transaction->currency_code);
  
  $form = confirm_form($form,
    t('Are you sure you want to void this transaction?'),
    'admin/commerce/orders/' . $order->order_id . '/payment',
    t('The amount of captured funds, <b>@void_amount @curr_code</b>, will be returned to the customer in a void transaction. Please note this action cannot be undone.', array('@void_amount' => $void_amount, '@curr_code' => $transaction->currency_code)),
    t('Void'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Validate handler for voiding, just ensure transaction has not expired
 */
function commerce_firstdata_cc_void_form_validate($form, &$form_state) {
  $transaction = $form_state['transaction'];

  // If the transaction has expired, display an error message and redirect.
  if (time() - $transaction->created > 86400 * 30) {
    drupal_set_message(t('This transaction has passed its 30 day limit cannot be voided.'), 'error');
    drupal_goto('admin/commerce/orders/' . $form_state['order']->order_id . '/payment');
  }
}

/**
 * Submit handler: process a transaction void
 */
function commerce_firstdata_cc_void_form_submit($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $payment_method = $form_state['payment_method'];
  $order = $form_state['order'];

  // build transaction XML
  $transaction_xml = '<order>';
  $transaction_xml .= commerce_firstdata_xml_merchantinfo($payment_method);
  $transaction_xml .= commerce_firstdata_xml_transcationdetails($order, $transaction->remote_id);
  $transaction_xml .= commerce_firstdata_xml_orderoptions($payment_method, COMMERCE_CREDIT_VOID);
  $transaction_xml .= '</order>';
  $transaction_xml = commerce_firstdata_clean_xml($transaction_xml);

  // Send the transcation to First Data
  $transaction_result = firstdata_commerce_send_transaction($transaction_xml, $payment_method);

  // Update and save the transaction based on the response.
  $transaction->payload[REQUEST_TIME] = $transaction_result['payload'];

  // If we didn't get an approval response code...
  if ($transaction_result['approved'] != 'APPROVED') {
    // Display an error message but leave the transaction pending.
    drupal_set_message(t('Void failed.'), 'error');
    drupal_set_message(check_plain($transaction_result['error']), 'error');
  }
  else {
    drupal_set_message(t('Voided transaction successfully.'));

    // Zero out the transaction amount
    $transaction->amount = commerce_currency_decimal_to_amount(0, $transaction->currency_code);

    // Set the remote and local status accordingly.
    $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
    $transaction->remote_status = commerce_firstdata_transaction_map(COMMERCE_CREDIT_VOID);

    // Append a capture indication to the result message.
    $transaction->message .= '<br />' . t('<b>Transaction Voided</b>: @date', array('@date' => format_date(REQUEST_TIME, 'short')));
  }

  commerce_payment_transaction_save($transaction);

  $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';
}

/**
 * Form callback: allows the user to credit funds back to a customer.
 */
function commerce_firstdata_cc_credit_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  $form_state['payment_method'] = $payment_method;

  $balance = commerce_payment_order_balance($order);

  // Set default amount to the amount left in the transaction
  $default_amount = $transaction->amount;

  // Convert the price amount to a user friendly decimal value.
  $default_amount = commerce_currency_amount_to_decimal($default_amount, $transaction->currency_code);

  $description = implode('<br />', array(
    t('Order balance: @balance', array('@balance' => commerce_currency_format($balance['amount'], $balance['currency_code']))),
    t('Funds remaining in transaction: @remaining', array('@remaining' => commerce_currency_format($transaction->amount, $transaction->currency_code))),
  ));

  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Credit amount'),
    '#description' => t('The amount entered will be returned to the customer. You cannot enter an amount greater than the remaining amount from the transaction.'),
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
 * Validate handler: ensure a valid amount is given.
 */
function commerce_firstdata_cc_credit_form_validate($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];

  // Ensure a positive numeric amount has been entered for capture.
  if (!is_numeric($amount) || $amount <= 0) {
    form_set_error('amount', t('You must specify a positive numeric amount to capture.'));
  }

  // Ensure the amount is less than or equal to the remaining transaction amount.
  if ($amount > commerce_currency_amount_to_decimal($transaction->amount, $transaction->currency_code)) {
    form_set_error('amount', t('You cannot credit more than what remains in the transaction balance.'));
  }

  // If the authorization has expired, display an error message and redirect.
  if (time() - $transaction->created > 86400 * 30) {
    drupal_set_message(t('This authorization has passed its 30 day limit cannot be captured.'), 'error');
    drupal_goto('admin/commerce/orders/' . $form_state['order']->order_id . '/payment');
  }
}

/**
 * Submit handler: process a credit transaction.
 */
function commerce_firstdata_cc_credit_form_submit($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];
  $payment_method = $form_state['payment_method'];
  $order = $form_state['order'];

  // build transaction XML
  $transaction_xml = '<order>';
  $transaction_xml .= commerce_firstdata_xml_merchantinfo($payment_method);
  $transaction_xml .= commerce_firstdata_xml_transcationdetails($order, $transaction->remote_id);
  $transaction_xml .= commerce_firstdata_xml_orderoptions($payment_method, COMMERCE_CREDIT_CREDIT);
  $transaction_xml .= commerce_firstdata_xml_payment($amount);
  $transaction_xml .= '</order>';
  $transaction_xml = commerce_firstdata_clean_xml($transaction_xml);

  // Send the transcation to First Data
  $transaction_result = firstdata_commerce_send_transaction($transaction_xml, $payment_method);

  // Update and save the transaction based on the response.
  $transaction->payload[REQUEST_TIME] = $transaction_result['payload'];

  // If we didn't get an approval response code...
  if ($transaction_result['approved'] != 'APPROVED') {
    // Display an error message but leave the transaction pending.
    drupal_set_message(t('Credit failed.'), 'error');
    drupal_set_message(check_plain($transaction_result['error']), 'error');
  }
  else {
    drupal_set_message(t('Credit successfull.'));

    // Update the transaction amount to the actual capture amount.
    $new_transaction_amount = $transaction->amount - commerce_currency_decimal_to_amount($amount, $transaction->currency_code);
    $transaction->amount = $new_transaction_amount;

    // Set the remote and local status accordingly.
    $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
    $transaction->remote_status = commerce_firstdata_transaction_map(COMMERCE_CREDIT_CREDIT);

    // Append a capture indication to the result message.
    $transaction->message .= '<br />' . t('<b>@amount Credited</b>: @date', array('@amount' => $amount.' '.$transaction->currency_code, '@date' => format_date(REQUEST_TIME, 'short')));
  }

  commerce_payment_transaction_save($transaction);

  $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';
}