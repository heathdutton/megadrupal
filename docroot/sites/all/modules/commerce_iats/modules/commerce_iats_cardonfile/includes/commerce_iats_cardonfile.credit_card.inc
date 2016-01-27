<?php
/**
 * @file
 * Implements iATS Payments credit card services for use in Drupal Commerce
 * with Commerce Card on File.
 */

/**
 * Payment method callback: settings form.
 */
function commerce_iats_cardonfile_credit_card_settings_form($settings = NULL) {
  module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');

  return commerce_iats_credit_card_settings_form_base($settings);
}

/**
 * Payment method callback: checkout form.
 */
function commerce_iats_cardonfile_credit_card_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  if (isset($payment_method['settings']['direct_post']['enable_direct_post']) && $payment_method['settings']['direct_post']['enable_direct_post'] == 1) {
    module_load_include('inc', 'commerce_iats', 'includes/commerce_iats.direct_post.forms');
    return commerce_iats_direct_post_credit_card_form($payment_method, $order);
  }
  else {
    module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');

    // Prepare the fields to include on the credit card form.
    $fields = array(
      'code' => '',
      'type' => commerce_iats_enabled_credit_card_types($payment_method),
    );

    return commerce_payment_credit_card_form($fields);
  }
}

/**
 * Payment method callback: checkout form validation.
 */
function commerce_iats_cardonfile_credit_card_submit_form_validate($payment_method, $pane_form, $pane_values, $order, $form_parents = array()) {
  if (!commerce_iats_validate_payment_method($payment_method)) {
    drupal_set_message(t('Unable to process payment. Please contact the site administrator.'), 'error');
    return FALSE;
  }

  // Skip normal validation if the customer specified a card on file.
  if (!empty($pane_values['cardonfile']) && ($pane_values['cardonfile'] !== 'new')) {
    return TRUE;
  }

  module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');

  // Validate the credit card fields.
  $settings = array(
    'form_parents' => array_merge($form_parents, array('credit_card')),
  );

  if (empty($pane_values['credit_card']['code']))
  {
    unset($pane_values['credit_card']['code']);
  }

  return commerce_payment_credit_card_validate($pane_values['credit_card'], $settings);
}

/**
 * Payment method callback: checkout form submission.
 */
function commerce_iats_cardonfile_credit_card_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {
  // Validate credit card charge.
  if (empty($charge['amount'])) {
    $order->revision = TRUE;
    $order->log = 'Order charge amount is empty.';
    commerce_order_save($order);

    watchdog('commerce_iats', 'Empty charge amount: @charge for order @order', array(
      '@charge' => '<pre>' . print_r($charge, TRUE) . '</pre>',
      '@order' => '<pre>' . print_r($order, TRUE) . '</pre>',
    ), WATCHDOG_DEBUG);
    return FALSE;
  }

  $payment_data = $pane_values;

  // If the user has opted to create a recurring payment during the checkout
  // process, create an iATS customer code with the recurring schedule
  // before processing the checkout purchase.
  if (isset($order->data['recurring']) && !empty($order->data['recurring']['schedule_type'])) {
    module_load_include('inc', 'commerce_iats_cardonfile', 'includes/commerce_iats_cardonfile.credit_card_customer_code');

    $cc_payment_methods = commerce_iats_get_all_payment_methods('iats_credit_card_customer_code');

    $recurring_payment_method = NULL;
    foreach ($cc_payment_methods as $cc_payment_method) {
      if (isset($cc_payment_method['settings']['allow_recurring']) && ($cc_payment_method['settings']['allow_recurring'] == 1)) {
        $recurring_payment_method = $cc_payment_method;
      }
    }

    if ($recurring_payment_method != NULL) {
      if (!commerce_iats_process_payment($recurring_payment_method, $payment_data, $order,
        NULL, 'commerce_iats_cardonfile_credit_card_create_customer_code')) {
        drupal_set_message(t('We were unable to process your recurring payment at this time.'), 'error');
        return FALSE;
      }
    }
    else {
      watchdog('commerce_iats', 'Cannot create recurring payment. No valid payment method.', array(), WATCHDOG_WARNING);
      drupal_set_message(t('We were unable to process your recurring payment at this time.'), 'error');
      return FALSE;
    }
  }

  if (isset($pane_values['cardonfile'])) {
    if ($pane_values['cardonfile'] === 'new') {
      if ($pane_values['credit_card']['cardonfile_store']) {
        // Create a new card on file record.
        return commerce_iats_process_payment($payment_method, $payment_data, $order, $charge, 'commerce_iats_cardonfile_credit_card_create_and_charge');
      }
    }
    else {
      // Get the card on file data.
      $card_data = commerce_cardonfile_load($pane_values['cardonfile']);

      // Fail now if it is no longer available or the card is inactive.
      if (empty($card_data) || $card_data->status == 0) {
        drupal_set_message(t('The requested card on file is no longer valid.'), 'error');
        return FALSE;
      }

      $card_data->code = $pane_values['credit_card']['code'];

      return commerce_iats_process_payment($payment_method, $card_data, $order, $charge, 'commerce_iats_cardonfile_credit_card_charge');
    }
  }

  // Customer has opted not to use card on file / card on file is not available.
  module_load_include('inc', 'commerce_iats', 'includes/commerce_iats.credit_card');
  return commerce_iats_process_payment($payment_method, $payment_data, $order, $charge, 'commerce_iats_process_credit_card_payment');
}

/**
 * Processes a credit card payment using a stored card on file.
 *
 * @param array $payment_method
 *   The payment method used during this transaction.
 * @param array $card_data
 *   Array of credit card data.
 * @param object $order
 *   The order processed during this transaction.
 * @param array $charge
 *   The transaction charge.
 *
 * @return Mixed
 *   API response array or boolean FALSE on failure.
 */
function commerce_iats_cardonfile_credit_card_charge($payment_method, $card_data, $order, $charge = NULL) {
  // Create and populate the API request.
  $invoice_number = $order->order_number;
  drupal_alter('commerce_iats_invoice_number', $invoice_number, $order);

  $request = array(
    'customerIPAddress' => ip_address(),
    'customerCode' => $card_data->remote_id,
    'invoiceNum' => t('@prefix@number', array(
      '@prefix' => $payment_method['settings']['invoice_prefix'],
      '@number' => $invoice_number,
    )),
    'mop' => commerce_iats_credit_card_mop($card_data->card_type),
    'total' => commerce_currency_amount_to_decimal($charge['amount'], $charge['currency_code']),
    'currency' => $charge['currency_code'],
    'comment' => COMMERCE_IATS_COMMENT_PREFIX,
  );

  // Get the API service.
  $iats = commerce_iats_get_service('ProcessLink', $payment_method);

  // Log the request.
  if ($payment_method['settings']['log']['request'] === 'request') {
    commerce_iats_log_request($payment_method,
      'commerce_iats_cardonfile_credit_card_charge', $request);
  }

  $response = $iats->processCreditCardWithCustomerCode($request);

  return $response;
}

/**
 * Processes a credit card charge and creates a card on file record.
 *
 * @param array $payment_method
 *   The payment method used during this transaction.
 * @param array $payment_data
 *   Payment data retrieved from the submit form.
 * @param object $order
 *   The order processed during this transaction.
 * @param array $charge
 *   The transaction charge.
 *
 * @return Mixed
 *   API response array or error string.
 */
function commerce_iats_cardonfile_credit_card_create_and_charge($payment_method, $payment_data, $order, $charge = NULL) {
  // Format credit card data.
  $formatted_card_data = commerce_iats_format_credit_card_data($payment_data['credit_card']);

  // Create and populate the API request.
  $invoice_number = $order->order_number;
  drupal_alter('commerce_iats_invoice_number', $invoice_number, $order);

  $request = array(
    'customerIPAddress' => ip_address(),
    'invoiceNum' => t('@prefix@number', array(
      '@prefix' => $payment_method['settings']['invoice_prefix'],
      '@number' => $invoice_number,
    )),
    'ccNum' => $formatted_card_data['number'],
    'ccExp' => $formatted_card_data['expiry'],
    'cvv2' => $payment_data['credit_card']['code'],
    'mop' => $formatted_card_data['type'],
    'total' => commerce_currency_amount_to_decimal($charge['amount'], $charge['currency_code']),
    'currency' => $charge['currency_code'],
    'comment' => COMMERCE_IATS_COMMENT_PREFIX,
  );

  // Add the billing information.
  commerce_iats_address($request, $order, 'commerce_customer_billing');

  // Get the API service.
  $iats = commerce_iats_get_service('ProcessLink', $payment_method);

  // Log the request.
  if ($payment_method['settings']['log']['request'] === 'request') {
    commerce_iats_log_request($payment_method,
      'commerce_iats_cardonfile_credit_card_create_and_charge', $request);
  }

  // Create new customer code and process the payment.
  $response = $iats->createCustomerCodeAndProcessCreditCard($request);

  if ($response != NULL) {
    $customer_code = trim($response['CUSTOMERCODE']);
    if (commerce_iats_cardonfile_can_create_cardonfile($customer_code)) {
      $payment_data['credit_card']['default'] = (isset($payment_data['cardonfile_instance_default'])) ?
        $payment_data['cardonfile_instance_default'] : FALSE;

      // Create a new card on file record for the new customer code.
      commerce_iats_cardonfile_create_cardonfile($order, $payment_method,
        $payment_data['credit_card'], $customer_code,
        $formatted_card_data['orig_type']);
    }
    else {
      drupal_set_message(t('Your credit card could not be stored for later use.'), 'error');
    }
  }

  return $response;
}

/**
 * Payment method callback: card update form.
 *
 * Updates credit card details stored with iATS Payments.
 */
function commerce_iats_cardonfile_credit_card_update($form, &$form_state, $payment_method, $card_data) {
  $card = array(
    'number' => '',
    'exp_month' => $card_data->card_exp_month,
    'exp_year' => $card_data->card_exp_year,
  );

  $formatted_card_data = commerce_iats_format_credit_card_data($card);

  $name_parts = explode(' ', $card_data->card_name);
  $first_name = array_shift($name_parts);
  $last_name = implode(' ', $name_parts);

  // Create and populate the API request.
  $request = array(
    'customerIPAddress' => ip_address(),
    'customerCode' => $card_data->remote_id,
    'firstName' => $first_name,
    'lastName' => $last_name,
    'creditCardExpiry' => $formatted_card_data['expiry'],
    'mop' => commerce_iats_credit_card_mop($card_data->card_type),
    'updateCreditCardNum' => 0,
    'comment' => COMMERCE_IATS_COMMENT_PREFIX,
  );

  if (isset($form_state['values']['credit_card']['recurring'])
    && !empty($form_state['values']['credit_card']['recurring']['schedule_type'])) {
    // Updating a recurring payment.

    // Get default recurring payment duration.
    $payment_duration = variable_get('commerce_iats_recurring_payment_duration',
      COMMERCE_IATS_RECURRING_PAYMENT_DURATION);
    $end_time = mktime(0, 0, 0, date('n'), date('j') + $payment_duration, date('y'));

    $request['recurring'] = 1;
    $request['beginDate'] = commerce_iats_get_formatted_date(time());
    $request['endDate'] = commerce_iats_get_formatted_date($end_time);
    $request['amount'] = $form_state['values']['credit_card']['recurring']['amount'];
    $request['scheduleType'] = $form_state['values']['credit_card']['recurring']['schedule_type'];
    $request['scheduleDate'] = $form_state['values']['credit_card']['recurring']['schedule_date'];
  }
  else {
    // Not a recurring payment or cancelling an existing recurring payment.
    $request['recurring'] = 0;
    $request['beginDate'] = commerce_iats_get_formatted_date(time());
    $request['endDate'] = commerce_iats_get_formatted_date(time());
    $request['amount'] = 0;
    $request['scheduleType'] = '';
    $request['scheduleDate'] = '';
  }

  // Get the API service.
  $iats = commerce_iats_get_service('CustomerLink', $payment_method);

  // Log the request.
  if ($payment_method['settings']['log']['request'] === 'request') {
    commerce_iats_log_request($payment_method,
      'commerce_iats_cardonfile_credit_card_update', $request);
  }

  // Process the credit card update.
  $response = $iats->updateCreditCardCustomerCode($request);

  $auth_result = (isset($response['AUTHORIZATIONRESULT'])) ?
    trim($response['AUTHORIZATIONRESULT']) : NULL;

  if ($auth_result == NULL) {
    $auth_result = $response;
  }

  if (substr($auth_result, 0, 2) != COMMERCE_IATS_TXN_OK) {
    watchdog('commerce_iats_cardonfile',
      'iATS error when updating customer code: @error',
      array(
        '@error' => $auth_result,
      )
    );
    return FALSE;
  }

  // Update the recurring payment record.
  $card_on_file_recurring = commerce_iats_cardonfile_recurring_load($card_data->card_id);

  $card_on_file_recurring->amount = $request['amount'];
  $card_on_file_recurring->schedule_type = $request['scheduleType'];
  $card_on_file_recurring->schedule_date = $request['scheduleDate'];
  commerce_iats_cardonfile_recurring_save($card_on_file_recurring);

  return TRUE;
}

/**
 * Payment method callback: card delete form.
 *
 * Deletes customer code and associated credit card details
 * stored with iATS Payments.
 * Will not delete customer codes that have been used in a payment transaction.
 */
function commerce_iats_cardonfile_credit_card_delete($form, &$form_state, $payment_method, $card_data) {
  // Create and populate the API request.
  $request = array(
    'customerIPAddress' => ip_address(),
    'customerCode' => $card_data->remote_id,
    'comment' => COMMERCE_IATS_COMMENT_PREFIX,
  );

  // Get the API service.
  $iats = commerce_iats_get_service('CustomerLink', $payment_method);

  // Log the request.
  if ($payment_method['settings']['log']['request'] === 'request') {
    commerce_iats_log_request($payment_method,
      'commerce_iats_cardonfile_credit_card_delete', $request);
  }

  // Process the credit card deletion.
  $response = $iats->deleteCustomerCode($request);

  $auth_result = (isset($response['AUTHORIZATIONRESULT'])) ?
    trim($response['AUTHORIZATIONRESULT']) : NULL;

  if ($auth_result == NULL) {
    $auth_result = $response;
  }

  if ((substr($auth_result, 0, 2) == COMMERCE_IATS_TXN_OK) || strstr($auth_result, 'THE CUSTOMER CODE HAS BEEN USED')) {
    return TRUE;
  }
  else {
    watchdog('commerce_iats_cardonfile',
      'iATS error when deleting customer code: @error',
      array(
        '@error' => $auth_result,
      )
    );

    drupal_set_message(
      t('We were unable to remove your card at this time.'),
      'error');
  }

  return FALSE;
}
