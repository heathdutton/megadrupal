<?php
/**
 * @file
 * Provide admin functionalities such as manual capture and cancel transactions.
 */

/**
 * Capture transactions.
 */
function commerce_firstdata_connect_capture_form($form, &$form_state, $order, $transaction) {
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

  $default_amount = number_format(commerce_currency_amount_to_decimal($default_amount, $transaction->currency_code), 2, '.', '');

  $description = t('Authorization: @amount,', array('@amount' => commerce_currency_format($transaction->amount, $transaction->currency_code))) .
    t('Order balance: @balance', array('@balance' => commerce_currency_format($balance['amount'], $balance['currency_code'])));

  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Capture amount'),
    '#description' => t('@description', array('@description' => $description)),
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
function commerce_firstdata_connect_capture_form_validate($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];
  // Ensure a positive numeric amount has been entered for capture.
  if (!is_numeric($amount) || $amount <= 0) {
    form_set_error('amount', t('You must specify a positive numeric amount to capture.'));
  }

  // Ensure the amount is less than or equal to the authorization amount.
  if ($amount > commerce_currency_amount_to_decimal($transaction->amount, $transaction->currency_code)) {
    form_set_error('amount', t('You cannot capture more than you authorized through Firstdata Connect.'));
  }

}

/**
 * Submit handler: process a prior authorization capture via Ogone.
 */
function commerce_firstdata_connect_capture_form_submit($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $order = commerce_order_load($transaction->order_id);
  $payment_methods = commerce_payment_method_instance_load($transaction->instance_id);

  $amount = commerce_currency_decimal_to_amount($form_state['values']['amount'], $transaction->currency_code);

  $api = commerce_firstdata_connect_api_object('', $payment_methods['instance_id']);
  if ($transaction->amount == commerce_currency_decimal_to_amount($form_state['values']['amount'], $transaction->currency_code)) {
    $result = $api->crossPayment($order, $transaction, 'postauth');
    $data = commerce_firstdata_connect_get_response_data($result);
    $response_data_codes = commerce_firstdata_connect_get_response_code($data);
    if ($result->status_message == 'OK' and $result->code == 200 and $response_data_codes['approval_code_status'] == 'Y') {
      $transaction->amount = commerce_currency_decimal_to_amount($form_state['values']['amount'], $transaction->currency_code);
      $transaction->remote_id = !empty($response_data_codes['remote_id']) ? $transaction->remote_id . '/' . $response_data_codes['remote_id'] : $transaction->remote_id;
      $transaction->status = COMMERCE_FIRSTDATA_CONNECT_CAPTURED;
      $transaction->message .= '<br />' . t('Captured: @date', array('@date' => format_date(REQUEST_TIME, 'short')));
      if (!empty($data['tdate'])) {
        $transaction->data['tdate'] = $data['tdate'];
      }
      commerce_payment_transaction_save($transaction);
      drupal_goto('admin/commerce/orders');
    }
    elseif ($response_data_codes['message'] == 'PostAuth already performed') {
      $transaction->status = COMMERCE_FIRSTDATA_CONNECT_CAPTURED;
      $transaction->message .= '<br />' . t('Captured: @date', array('@date' => format_date(REQUEST_TIME, 'short'))) .
                                '<br />' . t('Reference number: @ref_num', array('@ref_num' => $response_data_codes['reference_number']));
      commerce_payment_transaction_save($transaction);
      drupal_set_message(t('This order has been captured previously.'));
      drupal_goto('admin/commerce/orders');
    }
    else {
      drupal_set_message(t('An error occur.'));
      drupal_goto('admin/commerce/orders');
    }
  }
  elseif ($transaction->amount > commerce_currency_decimal_to_amount($form_state['values']['amount'], $transaction->currency_code)) {
    $charge = commerce_currency_decimal_to_amount($form_state['values']['amount'], $transaction->currency_code);

    $amount = new stdClass();
    $amount->amount = $charge;
    $amount->currency_code = $transaction->currency_code;
    $result = $api->crossPayment($order, $transaction, 'postauth', $amount);
    // Recover the amount transaction in case of failure of the capture.
    $data = commerce_firstdata_connect_get_response_data($result);
    if ($result->status_message == 'OK' and $result->code == 200) {
      // Create new transaction for the captured amount.
      $new_transaction = commerce_payment_transaction_new($transaction->payment_method, $order->order_id);
      $new_transaction->amount = commerce_currency_decimal_to_amount($form_state['values']['amount'], $transaction->currency_code);
      $new_transaction->remote_id = $transaction->remote_id . '/' . $response_data_codes['remote_id'];
      $new_transaction->status = COMMERCE_FIRSTDATA_CONNECT_CAPTURED;
      $new_transaction->currency_code = $transaction->currency_code;
      $new_transaction->instance_id = 'commerce_firstdata_connect_full_payment|commerce_payment_commerce_firstdata_connect_full_payment';
      $new_transaction->message .= '<br />' . t('Captured: @date', array('@date' => format_date(REQUEST_TIME, 'short')));
      if (!empty($data['tdate'])) {
        $new_transaction->data['tdate'] = $data['tdate'];
      }
      commerce_payment_transaction_save($new_transaction);

      $transaction->amount = $transaction->amount - $new_transaction->amount;
      $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
      commerce_payment_transaction_save($transaction);
      drupal_goto('admin/commerce/orders');
    }
  }
}

/**
 * Provide cancel transaction functionality.
 */
function commerce_firstdata_connect_cancel_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;
  $form['flag'] = array(
    '#type' => 'hidden',
    '#value' => 1,
  );

  $form = confirm_form($form,
      t('Are you sure you want to void this transaction?'),
      'admin/commerce/orders/' . $order->order_id . '/payment',
      'The void operation can be successful before the batch is processed (i.e. midnight of day of transaction)',
      t('Void'),
      t('Back'),
      'confirm'
  );
  // The void operation can be successful before the batch is
  // Processed (i.e. midnight of day of transaction).
  return $form;
}

/**
 * Cancel transaction.
 */
function commerce_firstdata_connect_cancel_form_submit($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  $order = $form_state['order'];
  $api = commerce_firstdata_connect_api_object('', $payment_method['instance_id']);
  // Perform maintenance request.
  $result = $api->crossPayment($order, $transaction, 'void');
  if (is_object($result) == TRUE or empty($result['approval_code'])) {
    $data = commerce_firstdata_connect_get_response_data($result);
  }
  else {
    $data = $result;
  }

  $response_data = commerce_firstdata_connect_get_response_code($data);
  if ($response_data['approval_code_status'] != 'Y') {
    drupal_set_message(t('@error', array('@error' => $response_data['message'])), 'error');
    return;
  }
  else {
    if ($result->status_message == 'OK' and $result->code == 200) {
      $transaction->status = COMMERCE_FIRSTDATA_CONNECT_CANCELED;
      $transaction->message .= '<br/>' . 'The transaction have been voided.';
      commerce_payment_transaction_save($transaction);
      drupal_set_message(t('The transaction has been canceled.'), 'status');
    }
    drupal_goto('admin/commerce/orders/' . $order->order_id . '/payment');
  }
}
