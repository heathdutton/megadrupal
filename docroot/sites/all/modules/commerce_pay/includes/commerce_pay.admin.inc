<?php

/**
 * @file
 * Administrative forms for the Commerce Pay module.
 */


/**
 * Form callback: allows the user to capture a prior authorization.
 */
function commerce_pay_capture_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  $form_state['payment_method'] = $payment_method;

  $order_balance = commerce_payment_order_balance($order);

  // Convert the balance to the transaction currency.
  if ($order_balance['currency_code'] != $transaction->currency_code) {
    $order_balance['amount'] = commerce_currency_convert($order_balance['amount'], $order_balance['currency_code'], $transaction->currency_code);
    $order_balance['currency_code'] = $transaction->currency_code;
  }

  if ($order_balance['amount'] > 0 && $order_balance['amount'] < $transaction->amount) {
    $default_amount = $order_balance['amount'];
  }
  else {
    $default_amount = $transaction->amount;
  }

  // Convert the price amount to a user friendly decimal value.
  $default_amount = number_format(commerce_currency_amount_to_decimal($default_amount, $transaction->currency_code), 2, '.', '');

  $description = implode('<br />', array(
    t('Max. authorization: @amount', array('@amount' => commerce_currency_format($transaction->amount, $transaction->currency_code))),
    t('Order balance: @balance', array('@balance' => commerce_currency_format($order_balance['amount'], $order_balance['currency_code']))),
  ));

  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Capture amount'),
    '#description' => filter_xss_admin($description),
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
    'commerce_pay_capture_confirm'
  );

  return $form;
}

/**
 * Validate handler: ensure a valid amount is given.
 */
function commerce_pay_capture_form_validate($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];

  // Ensure a positive numeric amount has been entered for capture.
  if (!is_numeric($amount) || $amount <= 0) {
    form_set_error('amount', t('You must specify a positive numeric amount to capture.'));
  }

  // Ensure the amount is less than or equal to the authorization amount.
  if ($amount > commerce_currency_amount_to_decimal($transaction->amount, $transaction->currency_code)) {
    form_set_error('amount', t('You cannot capture more than the authorized amount.'));
  }

  // If the authorization has expired, display an error message and redirect.
  if (REQUEST_TIME - $transaction->created > 86400 * 29) {
    drupal_set_message(t('This authorization has passed its 29 day limit and cannot be captured.'), 'error');
    drupal_goto('admin/commerce/orders/' . $form_state['order']->order_id . '/payment');
  }
}

/**
 * Submit handler: process a prior authorization capture via WPP.
 */
function commerce_pay_capture_form_submit($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];
  $order = $form_state['order'];
  $payment_method = $form_state['payment_method'];

  // Prepare data for USAePay API transaction.
  // API credentials data - payment settings.
  $payment_settings = $payment_method['settings'];
  $payment_settings['testmode'] = 0;
  $payment_settings['command'] = 'capture';

  // Build order data.
  $order_data = array();
  // Specify refnum of the transaction that you would like to capture.
  $order_data['refnum'] = $transaction->remote_id;

  // Call USAePay API transaction helper function.
  $usaepay_transaction = commerce_pay_usaepay_api_transaction($payment_settings, $order_data);
  $transaction->payload[REQUEST_TIME . '-capture'] = $usaepay_transaction;

  if ($usaepay_transaction->success) {
    $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
    // Append a capture indication to the result message.
    // Check if there is partial capture.
    if ($amount < commerce_currency_amount_to_decimal($transaction->amount, $transaction->currency_code)) {
      $partial_amount = commerce_currency_decimal_to_amount($amount, $transaction->currency_code);
      $transaction->message .= '<br />' . t('Partial captured: @amount from @transaction_amount, @date', array('@amount' => commerce_currency_format($partial_amount, $transaction->currency_code), '@transaction_amount' => commerce_currency_format($transaction->amount, $transaction->currency_code), '@date' => format_date(REQUEST_TIME, 'short')));
    }
    else {
      $transaction->message .= '<br />' . t('Captured: @date', array('@date' => format_date(REQUEST_TIME, 'short')));
    }
    // Update the transaction amount to the actual capture amount.
    $transaction->amount = commerce_currency_decimal_to_amount($amount, $transaction->currency_code);
    // Store the type of transaction in the remote status.
    $transaction->remote_status = $usaepay_transaction->command;
  }
  else {
    // Display a general error to the customer if we can not find the address.
    drupal_set_message($usaepay_transaction->error, 'error');
    // Provide a more descriptive error message in the failed transaction and
    // the watchdog.
    $transaction->payload[REQUEST_TIME] = array();
    $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
    $transaction->message = $usaepay_transaction->error;

    watchdog('commerce_pay', $usaepay_transaction->error, NULL, WATCHDOG_ERROR);
  }

  // Save the updated original transaction.
  commerce_payment_transaction_save($transaction);

  // Redirect back to the current order payment page.
  $form_state['redirect'] = 'admin/commerce/orders/' . $order->order_id . '/payment';
}

/**
 * Form callback: allows the user to void a transaction.
 */
function commerce_pay_void_form($form, &$form_state, $order, $transaction) {
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
    'commerce_pay_void_confirm'
  );

  return $form;
}

/**
 * Submit handler: process the void request.
 */
function commerce_pay_void_form_submit($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $payment_method = $form_state['payment_method'];

  // Prepare data for USAePay API transaction.
  // API credentials data - payment settings.
  $payment_settings = $payment_method['settings'];
  $payment_settings['testmode'] = 0;
  $payment_settings['command'] = 'void:release';

  // Build order data.
  $order_data = array();
  // Specify refnum of the transaction that you would like to capture.
  $order_data['refnum'] = $transaction->remote_id;

  // Call USAePay API transaction helper function.
  $usaepay_transaction = commerce_pay_usaepay_api_transaction($payment_settings, $order_data);
  $transaction->payload[REQUEST_TIME . '-void'] = $usaepay_transaction;

  if ($usaepay_transaction->success) {
    drupal_set_message(t('Transaction successfully voided.'));
    $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
    // Store the type of transaction in the remote status.
    $transaction->remote_status = $usaepay_transaction->command;

    // Update the transaction message to show that it has been voided.
    $transaction->message .= '<br />' . t('Voided: @date', array('@date' => format_date(REQUEST_TIME, 'short')));
  }
  else {
    drupal_set_message(t('Void failed: @reason', array('@reason' => $usaepay_transaction->error)), 'error');
    $transaction->message .= '<br />' . $usaepay_transaction->error;
    watchdog('commerce_pay', $usaepay_transaction->error, NULL, WATCHDOG_ERROR);
  }

  // Save the updated original transaction.
  commerce_payment_transaction_save($transaction);
  $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';
}

/**
 * Form callback: allows the user to refund (CreditVoid) a transaction.
 */
function commerce_pay_refund_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  $form_state['payment_method'] = $payment_method;

  // Use order balance for partial refunds.
  $refund_total = 0;
  $order_balance = commerce_payment_order_balance($order);
  if (!empty($order_balance)) {
    // Check for existing partial refunds.
    $query = new EntityFieldQuery();
    $refund_payments_ids = $query->entityCondition('entity_type', 'commerce_payment_transaction')
      ->propertyCondition('order_id', $order->order_id)
      ->propertyCondition('payment_method', 'commerce_pay')
      ->propertyCondition('transaction_id', $transaction->transaction_id, '<>')
      ->propertyCondition('amount', 0, '<')
      ->execute();
    if (!empty($refund_payments_ids)) {
      $refund_payments = entity_load('commerce_payment_transaction', array_keys($refund_payments_ids['commerce_payment_transaction']));
      foreach ($refund_payments as $refund_payment) {
        if (!empty($refund_payment->data) && !empty($refund_payment->data['orig_payment']) && $refund_payment->data['orig_payment'] == $transaction->transaction_id) {
          $refund_total += $refund_payment->amount;
        }
      }
    }
  }
  $default_amount = $transaction->amount + $refund_total;
  $default_amount = commerce_currency_amount_to_decimal($default_amount, $transaction->currency_code);

  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Credit amount'),
    '#description' => t('Enter the amount to be credited back to the original credit card.'),
    '#default_value' => $default_amount,
    '#field_suffix' => check_plain($transaction->currency_code),
    '#size' => 16,
  );

  $form = confirm_form($form,
    t('Are you sure that you want to refund this transaction?'),
    'admin/commerce/orders/' . $order->order_id . '/payment',
    '',
    t('Refund'),
    t('Cancel'),
    'commerce_pay_refund_confirm'
  );

  return $form;
}

/**
 * Validate handler: check the credit amount before attempting credit request.
 */
function commerce_pay_refund_form_validate($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];
  // Ensure a positive numeric amount has been entered for credit.
  if (!is_numeric($amount) || $amount <= 0) {
    form_set_error('amount', t('You must specify a positive numeric amount to credit.'));
  }
  // Ensure the amount is less than or equal to the captured amount.
  if ($amount > commerce_currency_amount_to_decimal($transaction->amount, $transaction->currency_code)) {
    form_set_error('amount', t('You cannot refund more than you captured.'));
  }
  // If the transaction is older than 120 days, display an error message
  // and redirect back.
  if (time() - $transaction->created > 86400 * 120) {
    drupal_set_message(t('This capture has passed its 120 day limit for issuing credits.'), 'error');
    drupal_goto('admin/commerce/orders/' . $form_state['order']->order_id . '/payment');
  }
}

/**
 * Submit handler: process the refund request.
 */
function commerce_pay_refund_form_submit($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];
  $order = $form_state['order'];
  $payment_method = $form_state['payment_method'];

  // Prepare data for USAePay API transaction.
  // API credentials data - payment settings.
  $payment_settings = $payment_method['settings'];
  $payment_settings['testmode'] = 0;
  $payment_settings['command'] = 'refund';

  // Build order data.
  $order_data = array();
  $order_data['amount'] = $amount;
  // Specify refnum of the transaction that you would like to capture.
  $order_data['refnum'] = $transaction->remote_id;

  // Call USAePay API transaction helper function.
  $usaepay_transaction = commerce_pay_usaepay_api_transaction($payment_settings, $order_data);
  $transaction->payload[REQUEST_TIME . '-refund'] = $usaepay_transaction;

  if ($usaepay_transaction->success) {
    $refund_amount = commerce_currency_decimal_to_amount($amount, $transaction->currency_code);
    drupal_set_message(t('Refund for @amount issued successfully', array('@amount' => commerce_currency_format($refund_amount, $transaction->currency_code))));
    // Create a new transaction to record the credit.
    $refund_transaction = commerce_payment_transaction_new('commerce_pay', $order->order_id);
    $refund_transaction->instance_id = $payment_method['instance_id'];
    $refund_transaction->remote_id = $usaepay_transaction->refnum;
    $refund_transaction->amount = $refund_amount * -1;
    $refund_transaction->currency_code = $transaction->currency_code;
    $refund_transaction->payload[REQUEST_TIME] = $usaepay_transaction;
    $refund_transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
    $refund_transaction->remote_status = $usaepay_transaction->command;
    $refund_transaction->message = t('Refounded to @remote_id.', array('@remote_id' => $transaction->remote_id));
    $refund_transaction->data['orig_payment'] = $transaction->transaction_id;
    // Save the credit transaction.
    commerce_payment_transaction_save($refund_transaction);
  }
  else {
    // Save the failure response message to the original transaction.
    $transaction->payload[REQUEST_TIME] = $usaepay_transaction;
    // Display a failure message and response reason from Authorize.net.
    drupal_set_message(t('Refund failed: @reason', array('@reason' => $usaepay_transaction->error)), 'error');
    // Add an additional helper message if the transaction hadn't settled yet.
    // to do.
    $transaction->message .= '<br />' . $usaepay_transaction->error;
    watchdog('commerce_pay', $usaepay_transaction->error, NULL, WATCHDOG_ERROR);
  }

  // Save the updated original transaction.
  commerce_payment_transaction_save($transaction);
  $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';
}
