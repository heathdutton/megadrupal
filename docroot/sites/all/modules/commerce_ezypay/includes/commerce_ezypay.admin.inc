<?php

/**
 * @file
 * Administrative forms for the Commerce Ezypay module.
 */


/**
 * Form callback: allows the user to capture a prior authorization.
 * @todo
 */
function commerce_ezypay_capture_form($form, &$form_state, $order, $transaction) {
  drupal_set_message(t('No functionality implemented'));
  return array();
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
    '#description' => check_plain($description),
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
 * @todo
 */
function commerce_ezypay_capture_form_validate($form, &$form_state) {
  return;
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];

  // Ensure a positive numeric amount has been entered for capture.
  if (!is_numeric($amount) || $amount <= 0) {
    form_set_error('amount', t('You must specify a positive numeric amount to capture.'));
  }

  // Ensure the amount is less than or equal to the authorization amount.
  if ($amount > commerce_currency_amount_to_decimal($transaction->amount, $transaction->currency_code)) {
    form_set_error('amount', t('You cannot capture more than you authorized through Authorize.Net.'));
  }

  // If the authorization has expired, display an error message and redirect.
  if (time() - $transaction->created > 86400 * 30) {
    drupal_set_message(t('This authorization has passed its 30 day limit cannot be captured.'), 'error');
    drupal_goto('admin/commerce/orders/' . $form_state['order']->order_id . '/payment');
  }
}

/**
 * Submit handler: process a prior authorization capture via AIM.
 * @todo
 */
function commerce_ezypay_capture_form_submit($form, &$form_state) {
  return;
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];

  // Build a name-value pair array for this transaction.
  $nvp = array(
    'x_type' => 'PRIOR_AUTH_CAPTURE',
    'x_trans_id' => $transaction->remote_id,
    'x_amount' => $amount,
  );

  // Submit the request to EzyPay.
  $response = commerce_ezypay_request($form_state['payment_method'], $nvp);

  // Update and save the transaction based on the response.
  $transaction->payload[REQUEST_TIME] = $response;

  // If we didn't get an approval response code...
  if ($response[0] != '1') {
    // Display an error message but leave the transaction pending.
    drupal_set_message(t('Prior authorization capture failed, so the transaction will remain in a pending status.'), 'error');
    drupal_set_message(check_plain($response[3]), 'error');
  }
  else {
    drupal_set_message(t('Prior authorization captured successfully.'));

    // Update the transaction amount to the actual capture amount.
    $transaction->amount = commerce_currency_decimal_to_amount($amount, $transaction->currency_code);

    // Set the remote and local status accordingly.
    $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
    $transaction->remote_status = $response[11];

    // Append a capture indication to the result message.
    $transaction->message .= '<br />' . t('Captured: @date', array('@date' => format_date(REQUEST_TIME, 'short')));
  }

  commerce_payment_transaction_save($transaction);

  $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';
}

/**
 * Form builder to configure the ezypay gpg settings
*/
function commerce_ezypay_gpg_admin_form($form, $form_state) {
  $form['commerce_ezypay_gpg_path'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Path to gpg key files'),
    '#default_value' => variable_get('commerce_ezypay_gpg_path', '/var/gpg'),
    '#size'          => 60,
    '#description'   => t('Enter the absolute path to your gpg keys, note that for security this should be outside your document root.'),
    '#maxlength'     => 128,
    '#required'      => TRUE,
  );
  
  return system_settings_form($form);
}

/**
 * Validation handler for path
*/
function commerce_ezypay_gpg_admin_form_validate($form, &$form_state) {
  $path = $form_state['values']['commerce_ezypay_gpg_path'];
  if (substr($path, -1) == '/') {
    //strip the trailing /
    $form_state['values']['commerce_ezypay_gpg_path'] = $path = substr($path, 0, -1);
  }
  if (file_check_location($path .'/dummy.txt', $_SERVER['DOCUMENT_ROOT'] . base_path())) {
    form_set_error('commerce_ezypay_gpg_path', t('You cannot use a path inside your document root.'));
  }
  if (!is_dir($path)) {
    form_set_error('commerce_ezypay_gpg_path', t('The entered path does not exist.'));    
  }
}
