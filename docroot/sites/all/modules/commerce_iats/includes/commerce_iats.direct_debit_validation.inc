<?php
/**
 * @file
 * Implements iATS Payments direct debit payment validation for use
 * in Drupal Commerce.
 */

/**
 * Payment method callback: settings form.
 */
function commerce_iats_direct_debit_validation_settings_form($settings = NULL) {
  $form = commerce_iats_settings_form_base($settings);

  $form['notice'] = array(
    '#type' => 'item',
    '#markup' => t('Direct Debit payment validation does not behave in the same
                  way as standard payment methods; it cannot be selected by a
                  user during the checkout process. See README.txt for
                  information on implementing a Direct Debit checkout process.'),
    '#weight' => -1,
  );

  // Direct Post Method is not available for Direct Debit.
  unset($form['direct_post']);
  unset($form['process_key']);

  // No invoice in Direct Debit validation.
  unset($form['invoice_prefix']);

  // Direct Debit limited to UK server.
  $form['server']['#options'] = array(
    COMMERCE_IATS_UK_SERVER_ID => t('UK'),
  );
  $form['server']['#default_value'] = COMMERCE_IATS_UK_SERVER_ID;

  $form['direct_debit'] = array(
    '#type' => 'fieldset',
    '#title' => 'Direct Debit',
  );

  $form['direct_debit']['info'] = array(
    '#type' => 'item',
    '#markup' => t('Your company must be registered as a Direct Debit service
                    user with a Bank or Building Society before you can process
                    Direct Debit payments.'),
  );

  $form['direct_debit']['service_user_number'] = array(
    '#type' => 'textfield',
    '#title' => 'Service User Number',
    '#size' => 32,
    '#required' => TRUE,
    '#default_value' => (isset($settings['direct_debit']['service_user_number'])) ?
    $settings['direct_debit']['service_user_number'] : '',
  );

  $form['direct_debit']['company_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Company Name'),
    '#size' => 32,
    '#default_value' => (isset($settings['direct_debit']['company_name'])) ?
    $settings['direct_debit']['company_name'] : '',
  );

  $form['direct_debit']['company_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Address'),
    '#size' => 64,
    '#default_value' => (isset($settings['direct_debit']['company_address'])) ?
    $settings['direct_debit']['company_address'] : '',
  );

  $form['direct_debit']['company_city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#size' => 32,
    '#default_value' => (isset($settings['direct_debit']['company_city'])) ?
    $settings['direct_debit']['company_city'] : '',
  );

  $form['direct_debit']['company_state'] = array(
    '#type' => 'textfield',
    '#title' => t('State'),
    '#size' => 32,
    '#default_value' => (isset($settings['direct_debit']['company_state'])) ?
    $settings['direct_debit']['company_state'] : '',
  );

  $form['direct_debit']['company_country'] = array(
    '#type' => 'textfield',
    '#title' => t('Country'),
    '#size' => 32,
    '#default_value' => (isset($settings['direct_debit']['company_country'])) ?
    $settings['direct_debit']['company_country'] : '',
  );

  $form['direct_debit']['company_postcode'] = array(
    '#type' => 'textfield',
    '#title' => t('ZIP / Post Code'),
    '#size' => 16,
    '#default_value' => (isset($settings['direct_debit']['company_postcode'])) ?
    $settings['direct_debit']['company_postcode'] : '',
  );

  $form['direct_debit']['company_phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone Number'),
    '#size' => 32,
    '#default_value' => (isset($settings['direct_debit']['company_phone'])) ?
    $settings['direct_debit']['company_phone'] : '',
  );

  $form['direct_debit']['company_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email Address'),
    '#size' => 32,
    '#default_value' => (isset($settings['direct_debit']['company_email'])) ?
    $settings['direct_debit']['company_email'] : '',
  );

  return $form;
}

/**
 * Payment method callback: checkout form submission.
 */
function commerce_iats_direct_debit_validation_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {
  $payment_data = $pane_values;

  return commerce_iats_process_payment($payment_method, $payment_data, $order,
    $charge, 'commerce_iats_validate_direct_debit_payment');
}

/**
 * Validates a direct debit payment via the iATS Payments API.
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
function commerce_iats_validate_direct_debit_payment($payment_method, $payment_data, $order, $charge) {
  $combined_address = $payment_data['payer']['address_1'] . "\n"
    . $payment_data['payer']['address_2'] . "\n"
    . $payment_data['payer']['address_3'];

  $account_num = $payment_data['account']['sort_code'] . $payment_data['account']['number'];
  $begin_date = $payment_data['account']['first_collection_date'];
  $end_date = $order->data['recurring']['end_date'];

  // Create and populate the API request.
  $request = array(
    'customerIPAddress' => ip_address(),
    'ACHEFTReferenceNum' => '',
    'beginDate' => $begin_date,
    'endDate' => $end_date,
    'accountCustomerName' => $payment_data['account']['account_holder'],
    'accountNum' => $account_num,
    'companyName' => $payment_data['payer']['company_name'],
    'firstName' => $payment_data['payer']['first_name'],
    'lastName' => $payment_data['payer']['last_name'],
    'address' => $combined_address,
    'city' => $payment_data['payer']['town'],
    'state' => $payment_data['payer']['county'],
    // Direct Debit is only available to the UK.
    'country' => 'UK',
    'email' => $payment_data['payer']['email'],
    'zipCode' => $payment_data['payer']['postcode'],
    'comment' => COMMERCE_IATS_COMMENT_PREFIX,
  );

  // Get the API service.
  $iats = commerce_iats_get_service('CustomerLink', $payment_method);
  if ($iats == NULL) {
    drupal_set_message(t('Unable to load iATS Payments library.'), 'error');
    return NULL;
  }

  // Log the request.
  if ($payment_method['settings']['log']['request'] === 'request') {
    commerce_iats_log_request($payment_method, 'commerce_iats_validate_direct_debit_payment', $request);
  }

  // Validate the direct debit payment.
  $response = $iats->directDebitACHEFTPayerValidate($request);

  return $response;
}
