<?php

/**
 * @file
 * Administrative forms for the BluePay standard module.
 */


/**
 * Form callback: allows the user to capture a prior authorization.
 */
function commerce_bluepay_capture_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  $form_state['payment_method'] = $payment_method;

  $balance = commerce_payment_order_balance($order);

  // Convert the balance to the transaction currency.
  if ($balance['currency_code'] != $transaction->currency_code) {
    $balance['amount'] = commerce_currency_convert($balance['amount'], $balance['currency_code'], $transaction->currency_code);
    $balance['currency_code'] = $transaction->currency_code;
  }

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
    '#disabled' => TRUE,
    '#size' => 16,
  );

  $form = confirm_form($form,
    t('Are you sure you want to capture?'),
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
function commerce_bluepay_capture_form_validate($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];

  // Ensure a positive numeric amount has been entered for capture.
  if (!is_numeric($amount) || $amount <= 0) {
    form_set_error('amount', t('You must specify a positive numeric amount to capture.'));
  }

  // If the authorization has expired, display an error message and redirect.
  if (REQUEST_TIME - $transaction->created > 86400 * 10) {
    drupal_set_message(t('This authorization has passed its 10 day limit and cannot be captured.'), 'error');
    drupal_goto('admin/commerce/orders/' . $form_state['order']->order_id . '/payment');
  }
}

/**
 * Submit handler: process a prior authorization capture via BluePay.
 */
function commerce_bluepay_capture_form_submit($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];

  $order = $form_state['order'];
  $payment_method = $form_state['payment_method'];

  // Prepare a capture request.
  $bp = new CommerceBluePay20Post(
    $payment_method['settings']['login'],
    $payment_method['settings']['tran_key'],
    $payment_method['settings']['txn_mode']
  );

  $bp->capture($transaction->remote_id);

  // Log the request if specified.
  if ($payment_method['settings']['log']['request'] == 'request') {
    watchdog('commerce_bluepay',
      'BluePay request: !param', array(
        '!param' => '<pre>' . print_r($bp->safeRequest(), TRUE) . '</pre>',
      ), WATCHDOG_DEBUG
    );
  }

  // Process the request and response.
  $response = $bp->process();

  // Log the response if specified.
  if ($payment_method['settings']['log']['response'] == 'response') {
    watchdog('commerce_bluepay',
      'BluePay response: !param',
      array(
        '!param' => '<pre>' . print_r($response, TRUE) . '</pre>',
      ), WATCHDOG_DEBUG
    );
  }

  $transaction->payload[REQUEST_TIME . '-capture'] = (array) $response;

  switch ($response->getStatus()) {
    case 1:
      drupal_set_message(t('Prior authorization captured successfully.'));

      // Update the original transaction amount to the actual capture amount,
      // its remote ID to the capture's transaction ID, and its statuses to
      // indicate successful payment.
      $transaction->remote_id = $response->getTransactionId();
      $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
      $transaction->remote_status = $response->getStatus();
      $transaction->data['bluepay']['transaction_type'] = COMMERCE_CREDIT_CAPTURE_ONLY;

      // Note the capture in the transaction message.
      $transaction->message .= '<br />' . t('Captured: @date', array('@date' => format_date(REQUEST_TIME, 'short')));
      break;

    default:
      // Display an error message but leave the transaction pending.
      drupal_set_message(t('Prior authorization capture failed, so the transaction will remain in a pending status.'), 'error');
      drupal_set_message(check_plain($response->getMessage()), 'error');
      break;
  }

  // Save the updated original transaction.
  commerce_payment_transaction_save($transaction);

  // Redirect back to the current order payment page.
  $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';
}


/**
 * Form callback: allows the user to void a transaction.
 */
function commerce_bluepay_void_form($form, &$form_state, $order, $transaction) {
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
function commerce_bluepay_void_form_submit($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);

  // Prepare a capture request.
  $bp = new CommerceBluePay20Post(
    $payment_method['settings']['login'],
    $payment_method['settings']['tran_key'],
    $payment_method['settings']['txn_mode']
  );

  $bp->void($transaction->remote_id);

  // Log the request if specified.
  if ($payment_method['settings']['log']['request'] == 'request') {
    watchdog('commerce_bluepay',
      'BluePay request: !param', array(
        '!param' => '<pre>' . print_r($bp->safeRequest(), TRUE) . '</pre>',
      ), WATCHDOG_DEBUG
    );
  }

  // Process the request and response.
  $response = $bp->process();

  // Log the response if specified.
  if ($payment_method['settings']['log']['response'] == 'response') {
    watchdog('commerce_bluepay',
      'BluePay response: !param',
      array(
        '!param' => '<pre>' . print_r($response, TRUE) . '</pre>',
      ), WATCHDOG_DEBUG
    );
  }

  // Update and save the transaction base on the response.
  $transaction->payload[REQUEST_TIME . '-void'] = (array) $response;

  // If we got an approval response code...
  if ($response->getStatus() == 1) {
    drupal_set_message(t('Transaction successfully voided.'));

    // Set the remote and Blocal status accordingly.
    $transaction->remote_id = $response->getTransactionId();
    $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
    $transaction->remote_status = 0;

    // Update the transaction message to show that it has been voided.
    $transaction->message .= '<br />' . t('Voided: @date', array('@date' => format_date(REQUEST_TIME, 'short')));
  }
  else {
    drupal_set_message(t('Prior authorization capture failed, so the transaction will remain in a pending status.'), 'error');
    drupal_set_message(check_plain($response->getMessage()), 'error');
  }

  commerce_payment_transaction_save($transaction);

  $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';
}

/**
 * Form callback: allows the user to issue a credit on a prior transaction.
 */
function commerce_bluepay_refund_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  $form_state['payment_method'] = $payment_method;

  $default_amount = number_format(commerce_currency_amount_to_decimal($transaction->amount, $transaction->currency_code), 2, '.', '');

  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Refund amount'),
    '#description' => t('Enter the amount to be refunded back to the original credit card.'),
    '#default_value' => $default_amount,
    '#field_suffix' => check_plain($transaction->currency_code),
    '#disabled' => TRUE,
    '#size' => 16,
  );

  $form = confirm_form($form,
    t('Are you sure you want to refund?'),
    'admin/commerce/orders/' . $order->order_id . '/payment',
    '',
    t('Refund'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Validate handler: check the credit amount before attempting a refund request.
 */
function commerce_bluepay_refund_form_validate($form, &$form_state) {
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

  // If the transaction is older than 60 days, display an error message and redirect.
  if (time() - $transaction->created > 86400 * 30) {
    drupal_set_message(t('This transaction has passed its 60 day limit for issuing refunds.'), 'error');
    drupal_goto('admin/commerce/orders/' . $form_state['order']->order_id . '/payment');
  }
}

/**
 * Submit handler: process a refund via BluePay.
 */
function commerce_bluepay_refund_form_submit($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];
  $order = $form_state['order'];
  $payment_method = $form_state['payment_method'];

  // Prepare a capture request.
  $bp = new CommerceBluePay20Post(
    $payment_method['settings']['login'],
    $payment_method['settings']['tran_key'],
    $payment_method['settings']['txn_mode']
  );

  $bp->refund($transaction->remote_id);

  // Log the request if specified.
  if ($payment_method['settings']['log']['request'] == 'request') {
    watchdog('commerce_bluepay',
      'BluePay request: !param', array(
        '!param' => '<pre>' . print_r($bp->safeRequest(), TRUE) . '</pre>',
      ), WATCHDOG_DEBUG
    );
  }

  // Process the request and response.
  $response = $bp->process();

  // Log the response if specified.
  if ($payment_method['settings']['log']['response'] == 'response') {
    watchdog('commerce_bluepay',
      'BluePay response: !param',
      array(
        '!param' => '<pre>' . print_r($response, TRUE) . '</pre>',
      ), WATCHDOG_DEBUG
    );
  }

  // If the credit succeeded...
  if ($response->getStatus() == 1) {
    $credit_amount = commerce_currency_decimal_to_amount($amount, $transaction->currency_code);
    drupal_set_message(t('Refund for @amount issued successfully.', array('@amount' => commerce_currency_format($credit_amount, $transaction->currency_code))));

    // Create a new transaction to record the credit.
    $credit_transaction = commerce_payment_transaction_new($payment_method['method_id'], $order->order_id);
    $credit_transaction->instance_id = $payment_method['instance_id'];
    $credit_transaction->remote_id = $response->getTransactionId();
    $credit_transaction->amount = $credit_amount * -1;
    $credit_transaction->currency_code = $transaction->currency_code;
    $credit_transaction->payload[REQUEST_TIME] = (array) $response;
    $credit_transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
    $credit_transaction->remote_status = $response->getStatus();
    $credit_transaction->message = t('Refunded to @remote_id.', array('@remote_id' => $transaction->remote_id));

    // Save the credit transaction.
    commerce_payment_transaction_save($credit_transaction);
    // Reset the remote status.
    $transaction->remote_status = 0;
    commerce_payment_transaction_save($transaction);
  }
  else {
    // Display a failure message and response reason from BluePay.
    drupal_set_message(t('Refund failed: @reason', array('@reason' => $response->getMessage())), 'error');

    // Save the failure response message to the original transaction.
    $transaction->payload[REQUEST_TIME . '-refund'] = (array) $response;
    commerce_payment_transaction_save($transaction);
  }

  $form_state['redirect'] = 'admin/commerce/orders/' . $order->order_id . '/payment';
}
