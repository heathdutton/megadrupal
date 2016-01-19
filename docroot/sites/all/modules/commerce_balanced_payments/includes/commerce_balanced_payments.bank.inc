<?php

/**
 * @file
 * Balanced Payments Bank Account Implementation.
 */

/**
 * Returns the default settings for the Balanced Payments Bank Account payment method.
 */
function commerce_balanced_payments_ba_default_settings() {
  return array(
    'aosa' => '',
    'sandbox_key' => '',
    'production_key' => '',
    'server' => 'sandbox',
    'callback' => '',
    'log' => array('request' => '0', 'response' => '0'),
    'completion_message' => array(
      'value' => '<h1>This order is not complete yet</h1>
<p>Your bank account is not yet verified. There will be two small deposits made to your bank account from Balanced Payments usually this takes between 1-2 days. Once those deposits show up in your account you can return to this site and verify your bank account. Once you verify your account all pending transactions related to the account will be processed immediately.</p>
<p><a href="/user/[commerce-order:uid]/bank-accounts">Manage your bank accounts</a></p>',
      'format' => 'full_html',
    ),
  );
}

/**
 * Bank Account Payment method callback: settings form.
 */
function commerce_balanced_payments_ba_settings_form($settings = NULL) {
  // Merge default settings into the stored settings array.
  $settings = (array) $settings + commerce_balanced_payments_ba_default_settings();

  $form = array();

  $form['aosa'] = array(
    '#type' => 'textfield',
    '#title' => t('Appear On Statement As'),
    '#description' => t('What charges by Drupal Commerce will appear on customers\' bank account statements as.'),
    '#default_value' => $settings['aosa'],
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

  $form['callback'] = array(
    '#type' => 'textfield',
    '#title' => t('Callback URL'),
    '#description' => t('If you want to add a callback URL to receive events via balanced payments you may enter a fully qualified url in this field, e.g. http://www.example.com/callback'),
    '#default_value' => $settings['callback'],
  );

  $form['completion_message'] = array(
    '#type' => 'text_format',
    '#title' => t('Checkout completion message'),
    '#default_value' => $settings['completion_message']['value'],
    '#format' => $settings['completion_message']['format'],
  );

  $var_info = array(
    'site' => array(
      'type' => 'site',
      'label' => t('Site information'),
      'description' => t('Site-wide settings and other global information.'),
    ),
    'commerce_order' => array(
      'label' => t('Order'),
      'type' => 'commerce_order',
    ),
  );

  $form['commerce_checkout_completion_message_help'] = RulesTokenEvaluator::help($var_info);

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
 * Bank Account Payment method callback: checkout form.
 */
function commerce_balanced_payments_ba_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  $form = array();

  // If the user has accounts on file.
  $balanced_commerce = new BalancedCommerce();
  $bank_accounts = $balanced_commerce->getBankAccountsByUser($order->uid);

  if (isset($bank_accounts)) {
    $options = array();

    foreach ($bank_accounts as $bank_account) {
      if ($bank_account->can_debit == TRUE) {
        $options[$bank_account->id] = $bank_account->account_number;
      }
    }

    if (empty($options)) {
      $form['new_account'] = _commerce_balanced_payments_get_checkout_fields();
      return $form;
    }

    $form['accounts_on_file'] = array(
      '#type' => 'select',
      '#title' => t('Verified Bank Accounts on File'),
      '#options' => $options,
      '#states' => array(
        'visible' => array(
          ':input[name="commerce_payment[payment_details][make_new]"]' => array('checked' => FALSE),
        ),
      ),
    );

    $form['make_new'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use a new Bank Account'),
    );

    $form['new_account'] = array(
      '#type' => 'container',
      '#states' => array(
        'visible' => array(
          ':input[name="commerce_payment[payment_details][make_new]"]' => array('checked' => TRUE),
        ),
      ),
    );

    $new_form = _commerce_balanced_payments_get_checkout_fields();

    foreach ($new_form as $k => $v) {
      $v['#states'] = array(
        'required' => array(
          ':input[name="commerce_payment[payment_details][make_new]"]' => array('checked' => TRUE),
        ),
      );

      $form['new_account'][$k] = $v;
    }
  }
  else {
    $form['new_account'] = _commerce_balanced_payments_get_checkout_fields();
  }

  return $form;
}

/**
 * Checkout form fields.
 *
 * @return array $form
 *   An array of form items.
 */
function _commerce_balanced_payments_get_checkout_fields() {
  $form = array();

  $form['account_type'] = array(
    '#type' => 'select',
    '#title' => t('Account Type'),
    '#options' => array(
      'checking' => t('Checking'),
      'savings' => t('Savings'),
    ),
  );

  $form['account_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Bank Account Number'),
  );

  $form['account_number_verify'] = array(
    '#type' => 'textfield',
    '#title' => t('Re-enter Bank Account Number'),
  );

  $form['routing_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Routing Number'),
  );

  $form['account_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name on Account'),
  );

  return $form;
}

/**
 * Bank Account Payment method callback: checkout form validation.
 */
function commerce_balanced_payments_ba_submit_form_validate($payment_method, $pane_form, $pane_values, $order, $form_parents = array()) {
  // Validate the bank account fields.
  if (isset($pane_values['make_new'])) {
    if ($pane_values['make_new'] == 0) {
      return TRUE;
    }
  }
  else {
    foreach ($pane_values['new_account'] as $k => $v) {
      $errors = array();

      if ($v == '') {
        form_set_error('new_account][' . $k, $pane_form['new_account'][$k]['#title'] . ' must be filled out');
        $errors[] = $k;
      }
    }

    if ($pane_values['new_account']['account_number'] != $pane_values['new_account']['account_number_verify']) {
      form_set_error('new_account][account_number', t('Account numbers need to match.'));
      $errors[] = 'not matching';
    }

    if (!empty($errors)) {
      return FALSE;
    }
  }
}

/**
 * Bank Account Payment method callback: checkout form submission.
 */
function commerce_balanced_payments_ba_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {
  // Initialize Balanced Payments API.
  $balanced_commerce = new BalancedCommerce();

  $account_on_file = FALSE;

  // If we are using an account on file charge that.
  if ($pane_values['make_new'] == 0 && isset($pane_values['accounts_on_file'])) {
    $account = $balanced_commerce->getBankAccountById($pane_values['accounts_on_file']);
    if ($account->can_debit == FALSE) {
      drupal_set_message('This Bank Account is not verified yet and can not be debited.', 'error');
      return FALSE;
    }

    $account_on_file = TRUE;
  }

  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

  // Build a description for the order.
  $description = array();

  // Descriptions come from products, though not all environments have them. So check first.
  if (function_exists('commerce_product_line_item_types')) {
    foreach ($order_wrapper->commerce_line_items as $delta => $line_item_wrapper) {
      if (in_array($line_item_wrapper->type->value(), commerce_product_line_item_types())) {
        $description[] = round($line_item_wrapper->quantity->value(), 2) . 'x ' . $line_item_wrapper->line_item_label->value();
      }
    }
  }

  // Prepare the billing address for use in the request.
  if ($order_wrapper->commerce_customer_billing->value()){
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

  // Check to see if the Customer exists in the database.
  $customer_exists = $balanced_commerce->customerExists($order->uid);

  if (isset($customer_exists)) {
    // Load the Customer.
    $customer = $customer_exists;
  }
  else {
    // Create the Customer.
    $user_full = user_load($order->uid);
    $customer = $balanced_commerce->createCustomer($user_full, $billing_address);
  }

  if ($account_on_file == FALSE) {
    // Create the Bank Account.
    $bank_info = array(
      'account_type' => $pane_values['new_account']['account_type'],
      'account_number' => $pane_values['new_account']['account_number'],
      'routing_number' => $pane_values['new_account']['routing_number'],
      'account_name' => $pane_values['new_account']['account_name'],
    );

    $bank_account = $balanced_commerce->createBankAccount($customer, $bank_info, $order->uid);

    // Initiate Verification.
    if (!isset($bank_account->status_code)) {
      $verification = $balanced_commerce->initiateVerification($bank_account);
    }
    else {
      return FALSE;
    }

    $nvp = array(
      'billing' => $billing_address,
      'charge' => $charge,
      'account_type' => $pane_values['new_account']['account_type'],
      'account_number' => $pane_values['new_account']['account_number'],
      'routing_number' => $pane_values['new_account']['routing_number'],
      'account_name' => $pane_values['new_account']['account_name'],
      'uid' => $order->uid,
    );

    // Response is the new bank account.
    $response = $bank_account;
  }
  else {
    $nvp = array(
      'billing' => $billing_address,
      'charge' => $charge,
      'uid' => $order->uid,
      'aid' => $pane_values['accounts_on_file'],
    );

    // Submit the request to Balanced Payments.
    $response = commerce_balanced_payments_ba_request($payment_method, $nvp, TRUE);
  }

  if ($response->_type == 'bank_account') {
    // Prepare a transaction object to log the API response.
    $transaction = commerce_payment_transaction_new('balancedpayments', $order->order_id);
    $transaction->instance_id = $payment_method['instance_id'];
    $transaction->remote_id = $response->id;
    $transaction->amount = $charge['amount'];
    $transaction->currency_code = $charge['currency_code'];
    $transaction->payload[REQUEST_TIME] = $response;

    // Set the transaction status to pending.
    $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;

    // Set the message.
    $transaction->message = t('PENDING: Waiting for Bank Account Verification.');

    // Save the transaction information.
    commerce_payment_transaction_save($transaction);

    drupal_set_message(t('Please come back after Balanced Payments has deposited two small amounts into your bank account to verify it.'), 'warning');
  }
  else {
    // Prepare a transaction object to log the API response.
    $transaction = commerce_payment_transaction_new('balancedpayments', $order->order_id);
    $transaction->instance_id = $payment_method['instance_id'];
    $transaction->remote_id = (!isset($response->id)) ? '' : $response->id;
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
      $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
    }

    // Build a meaningful response message.
    $transaction->message = t('SUCCESS: The transaction went through.');

    // Save the transaction information.
    commerce_payment_transaction_save($transaction);

    // If the payment failed, display an error and rebuild the form.
    if ($response->status != 'succeeded') {
      drupal_set_message(t('We received errors processing your bank account. Please enter your information again or try a different account.'), 'error');
      return FALSE;
    }
    else {
      drupal_set_message(t('Balanced Payments will make two small deposits into your account, you will need to verify those deposits before the transaction can be completed.'));
    }
  }
}

/**
 * Balanced Payments Bank Account Request.
 *
 * @param array $payment_method
 *   Array of payment method variables.
 * @param array $nvp
 *   Array of additional variables.
 *
 * @return object
 *   Response object.
 */
function commerce_balanced_payments_ba_request($payment_method, $nvp, $on_file = FALSE) {
  // Allow modules to alter parameters of the API request.
  drupal_alter('commerce_balanced_payments_ba_request', $nvp);

  $balanced_commerce = new BalancedCommerce();

  $billing = $nvp['billing'];
  $charge = $nvp['charge'];

  $customer_exists = $balanced_commerce->customerExists($nvp['uid']);

  if (!isset($customer_exists)) {
    // Create a Customer
    $user = user_load($nvp['uid']);
    $customer = $balanced_commerce->createCustomer($user, $billing);
  }
  else {
    $customer = $customer_exists;
  }

  if ($on_file == TRUE) {
    $debit = $balanced_commerce->debitBankAccount(
      $customer,
      $nvp['aid'],
      $charge['amount'],
      $text = 'Bank Account Debit',
      $description = ''
    );
  }
  else {
    // Create a bank account.
    $bank_info = array(
      'account_number' => $nvp['account_number'],
      'account_name' => $nvp['account_name'],
      'routing_number' => $nvp['routing_number'],
      'account_type' => $nvp['account_type'],
    );

    $bank_account = $balanced_commerce->createBankAccount($customer, $bank_info, $nvp['uid']);
    $verification = $balanced_commerce->initiateVerification($bank_account);

    $debit = $bank_account;
  }

  // Make the response an array.
  $response = $debit;

  // Log the response if specified.
  if ($payment_method['settings']['log']['response'] == 'response') {
    watchdog('commerce_balanced_payments', 'Balanced Payments response: !param', array('!param' => '<pre>' . check_plain(print_r($response, TRUE)) . '</pre>', WATCHDOG_DEBUG));
  }

  return $response;
}
