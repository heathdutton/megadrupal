<?php

/**
 * @file
 * Balanced Payments Credit Card Implementation.
 */

/**
 * Returns the default settings for the Balanced Payments payment method.
 */
function commerce_balanced_payments_default_settings() {
  return array(
    'aosa' => '',
    'sandbox_key' => '',
    'production_key' => '',
    'cardonfile' => FALSE,
    'txn_type' => COMMERCE_CREDIT_AUTH_CAPTURE,
    'server' => 'sandbox',
    'log' => array('request' => '0', 'response' => '0'),
    'card_types' => drupal_map_assoc(
      array(
        'visa',
        'mastercard',
        'amex',
        'discover',
      )
    ),
  );
}

/**
 * Payment method callback: settings form.
 */
function commerce_balanced_payments_settings_form($settings = NULL) {
  module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');

  // Merge default settings into the stored settings array.
  $settings = (array) $settings + commerce_balanced_payments_default_settings();

  $form = array();

  $form['aosa'] = array(
    '#type' => 'textfield',
    '#title' => t('Appear On Statement As'),
    '#description' => t("What charges by Drupal Commerce will appear on customers' credit card statements as."),
    '#default_value' => $settings['aosa'],
  );

  $form['txn_type'] = array(
    '#type' => 'radios',
    '#title' => t('Default credit card transaction type'),
    '#description' => t('The default will be used to process transactions during checkout.'),
    '#options' => array(
      COMMERCE_CREDIT_AUTH_CAPTURE => t('Authorization and capture'),
      COMMERCE_CREDIT_AUTH_ONLY => t('Authorization only (requires manual or automated capture after checkout)'),
    ),
    '#default_value' => $settings['txn_type'],
  );

  $form['sandbox_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Sandbox API Secret Key'),
    '#default_value' => $settings['sandbox_key'],
  );

  $form['production_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Production API Secret Key'),
    '#default_value' => $settings['production_key'],
  );

  $form['server'] = array(
    '#type' => 'radios',
    '#title' => t('Balanced Payments server'),
    '#options' => array(
      'sandbox' => ('Sandbox - use for testing.'),
      'production' => ('Production - use for processing real transactions'),
    ),
    '#default_value' => $settings['server'],
  );

  $form['card_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Limit accepted credit cards to the following types'),
    '#description' => t('If you want to limit acceptable card types, you should only select those supported by your merchant account.') . '<br />' . t('If none are checked, any credit card type will be accepted.'),
    '#options' => commerce_payment_credit_card_types(),
    '#default_value' => $settings['card_types'],
  );

  if (module_exists('commerce_cardonfile')) {
    $form['cardonfile'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable Card on File functionality with this payment method.'),
      '#default_value' => $settings['cardonfile'],
    );
  }
  else {
    $form['cardonfile'] = array(
      '#markup' => t('To enable Card on File funcitionality download and install the Card on File module.'),
    );
  }

  $form['log'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Log the following messages for debugging'),
    '#options' => array(
      'request' => t('API request messages'),
      'response' => t('API response messages'),
    ),
    '#default_value' => $settings['log'],
  );

  return $form;
}

/**
 * Payment method callback: checkout form.
 */
function commerce_balanced_payments_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');

  // Prepare the fields to include on the credit card form.
  $fields = array(
    'code' => '',
  );

  // Add the credit card types array if necessary.
  if (isset($payment_method['settings']['card_types'])) {
    $card_types = array_diff(array_values($payment_method['settings']['card_types']), array(0));

    if (!empty($card_types)) {
      $fields['type'] = $card_types;
    }
  }

  return commerce_payment_credit_card_form($fields);
}

/**
 * Payment method callback: checkout form validation.
 */
function commerce_balanced_payments_submit_form_validate($payment_method, $pane_form, $pane_values, $order, $form_parents = array()) {
  // If the customer specified a card on file, skip the normal validation.
  if (module_exists('commerce_cardonfile') && !empty($payment_method['settings']['cardonfile']) &&
      !empty($pane_values['cardonfile']) && $pane_values['cardonfile'] !== 'new') {
    return;
  }

  module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');

  // Validate the credit card fields.
  $settings = array(
    'form_parents' => array_merge($form_parents, array('credit_card')),
  );

  if (!commerce_payment_credit_card_validate($pane_values['credit_card'], $settings)) {
    return FALSE;
  }
}

/**
 * Payment method callback: checkout form submission.
 */
function commerce_balanced_payments_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {
  // If the customer specified payment using a card on file, attempt that now
  // and simply return the result.
  if (module_exists('commerce_cardonfile') && $payment_method['settings']['cardonfile'] &&
      !empty($pane_values['cardonfile']) && $pane_values['cardonfile'] !== 'new') {
    return commerce_balanced_payments_cof_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge);
  }

  // Determine the credit card type if possible for use in later code.
  if (!empty($pane_values['credit_card']['number'])) {
    module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');
    $card_type = commerce_payment_validate_credit_card_type($pane_values['credit_card']['number'], array_keys(commerce_payment_credit_card_types()));
  }

  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

  // Get the default transaction type from the payment method settings.
  $txn_type = $payment_method['settings']['txn_type'];

  // Build a description for the order.
  $description = array();
  // Descriptions come from products, though not all environments have them.
  if (function_exists('commerce_product_line_item_types')) {
    foreach ($order_wrapper->commerce_line_items as $delta => $line_item_wrapper) {
      if (in_array($line_item_wrapper->type->value(), commerce_product_line_item_types())) {
        $description[] = round($line_item_wrapper->quantity->value(), 2) . 'x ' . $line_item_wrapper->line_item_label->value();
      }
    }
  }

  // Prepare the billing address for use in the request.
  if ($order_wrapper->commerce_customer_billing->value()) {
    $billing_address = $order_wrapper->commerce_customer_billing->commerce_customer_address->value();

    if (empty($billing_address['first_name'])) {
      $name_parts = explode(' ', $billing_address['name_line']);
      $billing_address['first_name'] = array_shift($name_parts);
      $billing_address['last_name'] = implode(' ', $name_parts);
    }
  }
  else {
    $billing_address = array();
  }

  // Add the CVV if entered on the form.
  if (isset($pane_values['credit_card']['code'])) {
    $nvp['cvv2'] = $pane_values['credit_card']['code'];
  }

  $nvp += array(
    'billing' => $billing_address,
    'charge' => $charge,
    'card_num' => $pane_values['credit_card']['number'],
    'card_exp_month' => $pane_values['credit_card']['exp_month'],
    'card_exp_year' => $pane_values['credit_card']['exp_year'],
    'cardonfile' => isset($pane_values['credit_card']['cardonfile_store']) ? $pane_values['credit_card']['cardonfile_store'] : NULL,
    'uid' => $order->uid,
  );

  // Submit the request to Balanced Payments.
  $response = commerce_balanced_payments_request($payment_method, $nvp);

  if (isset($response->status_code)) {
    drupal_set_message($response->description, 'error');
    return FALSE;
  }
  else {
    // Prepare a transaction object to log the API response.
    $transaction = commerce_payment_transaction_new('balancedpayments', $order->order_id);
    $transaction->instance_id = $payment_method['instance_id'];
    $transaction->remote_id = $response->id;
    $transaction->amount = $charge['amount'];
    $transaction->currency_code = $charge['currency_code'];
    $transaction->payload[REQUEST_TIME] = $response;

    // If we didn't get an approval response code...
    if ($response->status != 'succeeded') {
      // Create a failed transaction with the error message.
      $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
    }
    else {
      // Set the transaction status based on the type of transaction this was.
      switch ($txn_type) {
        case COMMERCE_CREDIT_AUTH_ONLY:
          $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
          break;

        case COMMERCE_CREDIT_AUTH_CAPTURE:
          $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
          break;

        case COMMERCE_CREDIT_CAPTURE_ONLY:
          $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
          break;
      }
    }

    // Build a meaningful response message.
    $message = array(
      '<b>' . ($response->status != 'succeeded' ? t('REJECTED') : t('ACCEPTED')) . ':</b> ' . check_plain($response->_type),
    );

    // Add the CVV response if enabled.
    if (isset($nvp['cvv2'])) {
      $message[] = t('CVV match: @cvv', array('@cvv' => $response->source->security_code_check));
    }

    $transaction->message = implode('<br />', $message);

    // Save the transaction information.
    commerce_payment_transaction_save($transaction);
  }

  // If the payment failed, display an error and rebuild the form.
  if ($response->status != 'succeeded') {
    drupal_set_message(t('We received the following error processing your card. Please enter your information again or try a different card.'), 'error');
    return FALSE;
  }
}

/**
 * Balanced Payments payment method submission form callback.
 */
function commerce_balanced_payments_cof_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {
  // First attempt to load the card on file.
  $card_data = commerce_cardonfile_load($pane_values['cardonfile']);

  // Fail now if it is no longer available or the card is inactive.
  if (empty($card_data) || $card_data->status == 0) {
    drupal_set_message(t('The requested card on file is no longer valid.'), 'error');
    return FALSE;
  }

  return commerce_balanced_payments_cardonfile_charge($payment_method, $card_data, $order, $charge);
}

/**
 * Card on file callback: background charge payment.
 *
 * @param object $payment_method
 *   The payment method instance definition array.
 * @param object $card_data
 *   The stored credit card data array to be processed
 * @param object $order
 *   The order object that is being processed
 * @param array $charge
 *   The price array for the charge amount with keys of 'amount' and 'currency'
 *   If null the total value of the order is used.
 *
 * @return object
 *   TRUE if the transaction was successful
 */
function commerce_balanced_payments_cardonfile_charge($payment_method, $card_data, $order, $charge = NULL) {
  // Format order total for transaction.
  if (!isset($charge)) {
    $wrapper = entity_metadata_wrapper('commerce_order', $order);
    $charge = commerce_line_items_total($wrapper->commerce_line_items);
  }

  $remote_id = $card_data->remote_id;
  $uid = $card_data->uid;

  // Load the balanced payments library.
  $balanced_commerce = new BalancedCommerce();

  // Get the Customer.
  $customer = $balanced_commerce->customerExists($uid);

  // Debit the Customer.
  $debit = $balanced_commerce->debitCreditCard($customer, $remote_id, $charge['amount'], '', 'Card On File');

  $response = $debit;

  if (isset($response->status_code)) {
    drupal_set_message($response->description, 'error');
    return FALSE;
  }
  else {
    // Prepare a transaction object to represent the transaction attempt.
    $transaction = commerce_payment_transaction_new('balancedpayments', $order->order_id);
    $transaction->instance_id = $payment_method['instance_id'];
    $transaction->remote_id = $response->id;
    $transaction->amount = $charge['amount'];
    $transaction->currency_code = $charge['currency_code'];
    $transaction->payload[REQUEST_TIME] = $response;

    // If we didn't get an approval response code...
    if ($response->status != 'succeeded') {
      // Create a failed transaction with the error message.
      $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
    }
    else {
      // Set the transaction status based on the type of transaction this was.
      switch ($payment_method['settings']['txn_type']) {
        case COMMERCE_CREDIT_AUTH_ONLY:
          $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
          break;

        case COMMERCE_CREDIT_AUTH_CAPTURE:
          $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
          break;
      }
    }

    // Store the type of transaction in the remote status.
    $transaction->remote_status = $response->status;

    // Build a meaningful response message.
    $message = array(
      '<b>' . ($response->status != 'succeeded' ? t('REJECTED') : t('ACCEPTED')) . ':</b> ' . check_plain($response->_type),
    );

    // Add the CVV response if enabled.
    if (isset($nvp['cvv2'])) {
      $message[] = t('CVV match: @cvv', array('@cvv' => $response->source->security_code_check));
    }

    $transaction->message = implode('<br />', $message);

    // Save the transaction information.
    commerce_payment_transaction_save($transaction);

    // If the payment failed, display an error and rebuild the form.
    if ($response->status != 'succeeded') {
      drupal_set_message(t('We received the following error processing your card. Please enter your information again or try a different card.'), 'error');
      return FALSE;
    }

    return;
  }

  drupal_set_message(t('We could not process your card on file at this time. Please try again or use a different card.'), 'error');

  return FALSE;
}

/**
 * Card on file callback: updates the associated customer payment profile.
 */
function commerce_balanced_payments_cardonfile_update($form, &$form_state, $payment_method, $card_data) {
  // This currently doesn't work for Balanced Payments as they
  // don't allow updating Card data other than Meta data.
  return FALSE;
}

/**
 * Card on file callback: deletes the associated customer payment profile.
 */
function commerce_balanced_payments_cardonfile_delete($form, &$form_state, $payment_method, $card_data) {
  // Load the balanced payments library.
  $balanced_commerce = new BalancedCommerce();

  // Delete the card.
  $card = $balanced_commerce->deleteCreditCard($card_data->remote_id);

  return TRUE;
}

/**
 * Balanced Payments Credit Card Request.
 *
 * @param array $payment_method
 *   Array of payment method variables.
 * @param array $nvp
 *   Array of additional variables.
 *
 * @return object
 *   Response object.
 */
function commerce_balanced_payments_request($payment_method, $nvp) {
  // Allow modules to alter parameters of the API request.
  drupal_alter('commerce_balanced_payments_request', $nvp);

  $balanced_commerce = new BalancedCommerce();

  // Get/Create the Customer.
  $customer_exists = $balanced_commerce->customerExists($nvp['uid']);

  if (isset($customer_exists)) {
    // Load the Customer.
    $customer = $customer_exists;
  }
  else {
    // Create the Customer.
    $user_full = user_load($nvp['uid']);
    $customer = $balanced_commerce->createCustomer($user_full, $nvp['billing']);
  }

  // Create the Credit Card and add it to the customer.
  $card = $balanced_commerce->createCreditCard($customer, $nvp);

  $billing = $nvp['billing'];
  $charge = $nvp['charge'];

  // If no errors.
  if (!isset($card->status_code)) {
    // Debit the Customer.
    $debit = $balanced_commerce->debitCreditCard($customer, $card->id, $charge['amount'], '', 'Credit Card');

    if (isset($nvp['cardonfile']) && $nvp['cardonfile'] == 1) {
      // Save the card to file.
      $card_data = commerce_cardonfile_new();
      $card_data->uid = $nvp['uid'];
      $card_data->payment_method = $payment_method['method_id'];
      $card_data->instance_id = $payment_method['instance_id'];
      $card_data->remote_id = $card->id;
      $card_data->card_type = $card->brand;
      $card_data->card_name = !empty($billing['name_line']) ? $billing['name_line'] : '';
      $card_data->card_number = substr($nvp['card_num'], -4);
      $card_data->card_exp_month = $nvp['card_exp_month'];
      $card_data->card_exp_year = $nvp['card_exp_year'];
      $card_data->status = 1;

      // Save and log the creation of the new card on file.
      commerce_cardonfile_save($card_data);
    }

    $response = $debit;
  }
  else {
    $response = $card;
  }

  // Log the response if specified.
  if ($payment_method['settings']['log']['response'] == 'response') {
    watchdog('commerce_balanced_payments', 'Balanced Payments response: !param', array('!param' => '<pre>' . check_plain(print_r($response, TRUE)) . '</pre>', WATCHDOG_DEBUG));
  }

  return $response;
}
