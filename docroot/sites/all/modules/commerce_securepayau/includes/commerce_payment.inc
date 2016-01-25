<?php
/**
 * @file
 * Commerce Payment module integration.
 */

/**
 * Implements hook_commerce_payment_method_info().
 */
function commerce_securepayau_commerce_payment_method_info() {
  $payment_methods = array();

  $payment_methods['securepayau_xmlapi'] = array(
    'base' => 'commerce_securepayau_xmlapi',
    'title' => t('SecurePay.com.au - Credit Card'),
    'short_title' => t('SecurePay.com.au CC'),
    'display_title' => t('Credit card'),
    'description' => t('Implements the SecurePay.com.au payment services for use with Drupal Commerce.'),
  );

  drupal_alter('commerce_securepayau_commerce_payment_method_info', $payment_methods);

  return $payment_methods;
}

/**
 * Payment method callback: settings form.
 */
function commerce_securepayau_xmlapi_settings_form($settings = NULL) {
  $form = array();

  $form['credentials'] = array(
    '#type' => 'fieldset',
    '#title' => t('API Merchant ID and password'),
    '#description' => t('This information is required for Drupal Commerce to interact with your payment gateway account. It is different from your login ID and password and may be found through your account settings page.'),
    '#element_validate' => array('commerce_securepayau_xmlapi_settings_form_credentials_validate'),
  );
  $form['credentials']['merchant_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant ID'),
    '#default_value' => isset($settings['credentials']['merchant_id']) ? $settings['credentials']['merchant_id'] : '',
    '#required' => TRUE,
  );
  $form['credentials']['password'] = array(
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#default_value' => isset($settings['credentials']['password']) ? $settings['credentials']['password'] : '',
    '#required' => TRUE,
  );

  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mode/Settings'),
    '#description' => t('This allows you to set settings for the securepay account you are accessing.'),
  );
  $form['settings']['currency'] = array(
    '#type' => 'select',
    '#title' => 'Currency',
    '#options' => array(
      'AUD' => 'AUD',
      'USD' => 'USD'
    ),
    '#default_value' => isset($settings['settings']['currency']) ? $settings['settings']['currency'] : '',
  );
  $account_types = array(
    'live' => t('Live transactions in a live account'),
    'test' => t('Developer test account transactions'),
  );
  $form['settings']['txn_mode'] = array(
    '#type' => 'select',
    '#title' => t('Live/Test account'),
    '#description' => t('Set to use the live or test account settings at securepay.com.au.'),
    '#options' => $account_types,
    '#default_value' => isset($settings['settings']['txn_mode']) ? $settings['settings']['txn_mode'] : 'test',
  );
  $form['settings']['gateway_urls'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Gateway URL'),
    '#description' => t("You shouldn't need to update these, they should just work. The are made available incase securepay decides to change their domain and you need to be able to switch this<br/><br/>Securepay have different gateway URLs for different message types. The last part of the URL indicates the type of message so you do not need to enter that here as that will be added by this module at the time of payment request.<br/><br/>Ie:<br/>For the standard payment gateway it is accessed at:<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;https://www.securepay.com.au/xmlapi/payment<br/><br/>The 'payment' part is added by this module so you should just enter:<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;https://www.securepay.com.au/xmlapi/"),
  );
  $account_url = array(
    'live' => 'https://www.securepay.com.au/xmlapi/',
    'test' => 'https://test.securepay.com.au/xmlapi/',
  );
  foreach ($account_types as $type => $name) {
    $form['settings']['gateway_urls'][$type] = array(
      '#type' => 'textfield',
      '#title' => check_plain($name),
      '#default_value' => isset($settings['settings']['gateway_urls'][$type]) ? $settings['settings']['gateway_urls'][$type] : $account_url[$type],
    );
  }

  drupal_alter('commerce_securepayau_xmlapi_settings_form', $form, $settings);

  return $form;
}

/**
 * Validation callback for settings form credentials.
 */
function commerce_securepayau_xmlapi_settings_form_credentials_validate($element, &$form_state) {
  $payment_method = $form_state['values']['parameter']['payment_method']['settings']['payment_method'];
  $credentials = $payment_method['settings']['credentials'];

  $payment_method['settings']['credentials'] = $credentials;

  $xml = _commerce_securepayau_xmlapi_wrapper($payment_method, 'Echo');
  $response = commerce_securepayau_xmlapi($payment_method, 'Payment', '', $xml);

  // Parse the response.
  $response = _commerce_securepayau_parse_response($response);

  if (isset($response['statusCode'])) {
    switch ($response['statusCode']) {
      case 504:
        form_error($element, $response['statusDescription']);
    }
  }
}

/**
 * Payment method callback: checkout form.
 */
function commerce_securepayau_xmlapi_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');
  return commerce_payment_credit_card_form(array('code' => ''));
}


/**
 * Payment method callback: checkout form validation.
 */
function commerce_securepayau_xmlapi_submit_form_validate($payment_method, $pane_form, $pane_values, $order, $form_parents = array()) {
  $function = "_commerce_securepayau_xmlapi_submit_form_validate";

  // Allow other modules to alter the payment function to be invoked.
  drupal_alter('commerce_securepayau_xmlapi_submit_form_validate', $function, $payment_method, $pane_values);

  if (function_exists($function)) {
    return $function($payment_method, $pane_form, $pane_values, $order, $form_parents);
  }
  return FALSE;
}

function _commerce_securepayau_xmlapi_submit_form_validate($payment_method, $pane_form, $pane_values, $order, $form_parents) {
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
function commerce_securepayau_xmlapi_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {
  $function = "_commerce_securepayau_xmlapi_submit_form_submit";

  // Allow other modules to alter the payment function to be invoked.
  drupal_alter('commerce_securepayau_xmlapi_submit_form_submit', $function, $payment_method, $pane_values);

  if (function_exists($function)) {
    return $function($payment_method, $pane_form, $pane_values, $order, $charge);
  }
  return FALSE;
}

function _commerce_securepayau_xmlapi_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {
  $request = _commerce_securepayau_xmlapi_request_payment($pane_values, $order, $charge);
  $xml = _commerce_securepayau_xmlapi_wrapper($payment_method, 'Payment', _commerce_securepayau_array_to_xml($request));
  $response = commerce_securepayau_xmlapi($payment_method, 'Payment', '', $xml);

  // Parse the response.
  $response = _commerce_securepayau_parse_response($response);

  // Prepare a transaction object to log the API response.
  $transaction = commerce_payment_transaction_new('securepayau_xml', $order->order_id);
  $transaction->instance_id = $payment_method['instance_id'];
  $transaction->remote_id = isset($response['txnID']) ? $response['txnID'] : NULL;
  $transaction->amount = $charge['amount'];
  $transaction->currency_code = $charge['currency_code'];
  $transaction->payload[REQUEST_TIME] = $response;
  $transaction->status = isset($response['approved']) && $response['approved'] == 'Yes' ? COMMERCE_PAYMENT_STATUS_SUCCESS : COMMERCE_PAYMENT_STATUS_FAILURE;

  // Save the transaction information.
  commerce_payment_transaction_save($transaction);

  // If the payment failed, display an error and rebuild the form.
  if (!isset($response['approved']) || (isset($response['approved']) && $response['approved'] != 'Yes')) {
    // SecurePay recommends handling transactions with declined messages < 93 by saying card has been declined,
    // Please contact the bank and for codes above 93, please contact the merchant.
    $error_message = variable_get('commerce_securepayau_transactionerror_message', t('We received an error while processing your card. Please contact the merchant to verify your transaction.'));
    if (isset($response['responseCode']) && $response['responseCode'] <= 93) {
      $error_message = variable_get('commerce_securepayau_transactiondeclined_message', t('We received an error while processing your card. Please enter your information again, try a different card or contact your bank.'));
    }
    drupal_set_message($error_message, 'error');
    return FALSE;
  }

  module_invoke_all('commerce_securepayau_payment_successful', $payment_method, $pane_values, $order, $charge);

  return TRUE;
}

