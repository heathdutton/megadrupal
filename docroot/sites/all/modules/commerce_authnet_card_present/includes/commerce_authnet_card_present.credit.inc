<?php

/**
 * @file
 * Credit forms for Authorize.Net Card Present module.
 */

/**
 * Form callback: allows the user to issue a credit on a prior transaction.
 */
function commerce_authnet_card_present_aim_credit_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  $form_state['payment_method'] = $payment_method;

  $currency_data = commerce_currency_load($transaction->currency_code);
  $default_amount = number_format(commerce_currency_amount_to_decimal($transaction->amount, $transaction->currency_code), $currency_data['decimals'], $currency_data['decimal_separator'], $currency_data['thousands_separator']);

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
 * Validate handler: check the credit amount before attempting credit request.
 */
function commerce_authnet_card_present_aim_credit_form_validate($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];

  // Ensure a positive numeric amount has been entered for credit.
  if ($amount !== '' && (!is_numeric($amount) || intval($amount) != $amount || $amount <= 0)) {
    form_set_error('amount', t('You must specify a positive numeric amount to credit.'));
  }

  // Ensure the amount is less than or equal to the captured amount.
  if ($amount > commerce_currency_amount_to_decimal($transaction->amount, $transaction->currency_code)) {
    form_set_error('amount', t('You cannot credit more than you captured through Authorize.Net.'));
  }

  // Display error message and redirect if transaction is older than 120 days.
  if (time() - $transaction->created > 86400 * 120) {
    drupal_set_message(t('This capture has passed its 120 day limit for issuing credits.'), 'error');
    drupal_goto('admin/commerce/orders/' . $form_state['order']->order_id . '/payment');
  }
}

/**
 * Submit handler: process a credit via AIM.
 */
function commerce_authnet_card_present_aim_credit_form_submit($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $currency_data = commerce_currency_load($transaction->currency_code);
  $amount = number_format($form_state['values']['amount'], $currency_data['decimals'], $currency_data['decimal_separator'], $currency_data['thousands_separator']);
  $order = $form_state['order'];
  $payment_method = $form_state['payment_method'];

  // Determine the last 4 credit card digits from the previous transaction.
  $transaction_payload = end($transaction->payload);
  $credit_card = !empty($transaction_payload[20]) ? substr($transaction_payload[20], 4, 8) : '';

  // Make sure that the last 4 digits are available and valid.
  if (!intval($credit_card) || strlen($credit_card) != 4) {
    drupal_set_message(t('The credit could not be attempted, because the last 4 digits of the credit card were not found in the transaction data. Please login to your Authorize.Net merchant interface to issue the credit.'));
    return FALSE;
  }
  else {
    // Build a name-value pair array for this transaction.
    $nvp = array(
      'x_type' => 'CREDIT',
      'x_ref_trans_id' => $transaction->remote_id,
      'x_amount' => $amount,
      'x_card_num' => $credit_card,
      'x_invoice_num' => $order->order_number,
      'x_email' => substr($order->mail, 0, 255),
      'x_cust_id' => substr($order->uid, 0, 20),
      'card_present' => TRUE,
    );

    // Submit the request to Authorize.Net.
    $response = commerce_authnet_aim_request($form_state['payment_method'], $nvp);

    // If the credit succeeded...
    if ($response[1] == 1) {
      $credit_amount = commerce_currency_decimal_to_amount($amount, $transaction->currency_code);
      drupal_set_message(t('Credit for @amount issued successfully', array('@amount' => commerce_currency_format($credit_amount, $transaction->currency_code))));

      // Create a new transaction to record the credit.
      $credit_transaction = commerce_payment_transaction_new('authnet_aim_card_present', $order->order_id);
      $credit_transaction->instance_id = $payment_method['instance_id'];
      $credit_transaction->remote_id = $response[7];
      $credit_transaction->amount = $credit_amount * -1;
      $credit_transaction->currency_code = $transaction->currency_code;
      $credit_transaction->payload[REQUEST_TIME] = $response;
      $credit_transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
      $credit_transaction->remote_status = $response[11];
      $credit_transaction->message = t('Credited to @remote_id.', array('@remote_id' => $transaction->remote_id));

      // Save the credit transaction.
      commerce_payment_transaction_save($credit_transaction);
    }
    else {
      // Save the failure response message to the original transaction.
      $transaction->payload[REQUEST_TIME] = $response;

      // Display a failure message and response reason from Authorize.net.
      drupal_set_message(t('Credit failed: @reason', array('@reason' => $response[3])), 'error');

      // Add an additional helper message if the transaction hadn't settled yet.
      if ($response[2] == 54) {
        drupal_set_message(t('The transaction must be settled before a credit can be issued. This usually takes 24 hours'), 'error');
      }

      commerce_payment_transaction_save($transaction);
    }
  }

  $form_state['redirect'] = 'admin/commerce/orders/' . $order->order_id . '/payment';
}
