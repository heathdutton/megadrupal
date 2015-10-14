<?php

/**
 * Returns the default settings for the PayPal WPP payment method.
 */
function commerce_conekta_cc_default_settings() {
  $default_currency = commerce_default_currency();

  return array(
    'api_key_public_test' => '',
    'api_key_private_test' => '',
    'server' => 'sandbox',
    'code' => TRUE,
    'currency_code' => in_array($default_currency, array_keys(commerce_conekta_currencies())) ? $default_currency : 'USD',
    'allow_supported_currencies' => FALSE,
  );
}

/**
 * Payment method callback: settings form.
 */
function commerce_conekta_cc_settings_form($settings = array()) {
  module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');
  $form = array();

  // Merge default settings into the stored settings array.
  $settings = (array) $settings + commerce_conekta_cc_default_settings();

  $form['api_key_private_test'] = array(
    '#type' => 'textfield',
    '#title' => t('API private key test'),
    '#default_value' => $settings['api_key_private_test'],
  );

  $form['api_key_public_test'] = array(
    '#type' => 'textfield',
    '#title' => t('API public key test'),
    '#default_value' => $settings['api_key_public_test'],
  );

  $form['api_key_private_live'] = array(
    '#type' => 'textfield',
    '#title' => t('API private key live'),
    '#default_value' => $settings['api_key_private_live'],
  );

  $form['api_key_public_live'] = array(
    '#type' => 'textfield',
    '#title' => t('API public key live'),
    '#default_value' => $settings['api_key_public_live'],
  );

  $form['currency_code'] = array(
    '#type' => 'select',
    '#title' => t('Default currency'),
    '#description' => t('Transactions in other currencies will be converted to this currency, so multi-currency sites must be configured to use appropriate conversion rates.'),
    '#options' => array('USD' => 'USD', 'MXN' => 'MXN'),
    '#default_value' => $settings['currency_code'],
  );

  $form['server'] = array(
    '#type' => 'radios',
    '#title' => t('Environment'),
    '#options' => array(
      'sandbox' => ('Test - use for testing'),
      'live' => ('Live - use for processing real transactions'),
    ),
    '#default_value' => $settings['server'],
  );

  return $form;
}

/**
 * Payment method callback: checkout form.
 */
function commerce_conekta_cc_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');
  $payment_method['settings'] += commerce_conekta_cc_default_settings();

  // Prepare the fields to include on the credit card form.
  $fields = array();

  // Include the card security code field if specified.
  if ($payment_method['settings']['code']) {
    $fields['code'] = '';
  }

  // Add the start date and issue number if processing a Maestro or Solo card.
  $fields += array(
    'start_month' => '',
    'start_year' => '',
    'owner' => '',
  );

  $form = commerce_payment_credit_card_form($fields);

  //foreach (element_children($form['credit_card']) as $element_key) {
  //  $form['credit_card'][$element_key]['#name'] = '';
  //}

  // Input hidden where store the credit card conekta token
  $form['token'] = array(
    '#type' => 'hidden',
    '#value' => '',
  );

  // Add the data-conekta attribute so Conekta could detect the form fields
  $form['credit_card']['owner']['#attributes'] = array('data-conekta' => 'card[name]');
  $form['credit_card']['number']['#attributes'] = array('data-conekta' => 'card[number]');
  $form['credit_card']['exp_month']['#attributes'] = array('data-conekta' => 'card[exp_month]');
  $form['credit_card']['exp_year']['#attributes'] = array('data-conekta' => 'card[exp_year]');
  $form['credit_card']['code']['#attributes'] = array('data-conekta' => 'card[cvc]');

  // Add the Conekta JS
  $form['credit_card']['#attached']['js'][] = 'https://conektaapi.s3.amazonaws.com/v0.3.2/js/conekta.js';
  $form['credit_card']['#attached']['js'][] = drupal_get_path('module', 'commerce_conekta') . '/js/commerce_conekta_cc.js';

  // Pass the corresponding key depending if environment is test or live
  if ($payment_method['settings']['server'] == 'sandbox') {
    $public_key = $payment_method['settings']['api_key_public_test'];
  }
  else {
    $public_key = $payment_method['settings']['api_key_public_live'];
  }

  // Add the Conekta settings
  $params = array(
    'commerce_conekta_cc' => array(
      'public_key' => $public_key,
    ),
  );

  drupal_add_js($params, 'setting');

  return $form;
}

/**
 * Payment method callback: checkout form validation.
 */
function commerce_conekta_cc_submit_form_validate($payment_method, &$pane_form, $pane_values, $order, $form_parents = array()) {
  foreach (element_children($pane_form['credit_card']) as $element_key) {
    $pane_form['credit_card'][$element_key]['#required'] = FALSE;
  }

  // Need to use _POST due token is received empty on $pane_values
  $token = isset($_POST['commerce_payment']['payment_details']['token']) ? $_POST['commerce_payment']['payment_details']['token'] : NULL;
  $card_element_id = implode('][', $form_parents) . ']';
  if (empty($token)) {
    form_set_error($card_element_id, t('The credit card data was not correctly processed, please try again.'));
    return FALSE;
  }
}

/**
 * Payment method callback: checkout form submission.
 */
function commerce_conekta_cc_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {
  global $language;
  $token = isset($_POST['commerce_payment']['payment_details']['token']) ? $_POST['commerce_payment']['payment_details']['token'] : NULL;
  // Determine the order total
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $order_total = $order_wrapper->commerce_order_total->value();

  // Display an error and prevent the payment attempt if PayPal WPP has not been
  // configured yet.
  if (empty($payment_method['settings'])) {
    drupal_set_message(t('This payment method must be configured by an administrator before it can be used.'), 'error');
    return FALSE;
  }

  // Use of the default currency happens for orders that are calculated in currencies
  // not supported by Conekta.
  $currency_code = $order_total['currency_code'];
  $amount = $order_total['amount'];

  if (!in_array($order_total['currency_code'], array_keys(commerce_conekta_currencies()))) {
    $currency_code = $payment_method['settings']['currency_code'];
    // Convert the charge amount to default currency.
    $amount = commerce_currency_convert($order_total['amount'], $order_total['currency_code'], $currency_code);
  }

  // Ensure there is a billing address
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $valid_billing_address = TRUE;

  if (empty($order->commerce_customer_billing) || empty($order_wrapper->commerce_customer_billing->value()->commerce_customer_address)) {
    $valid_billing_address = FALSE;
  }
  else {
    // Check the values in the billing address array required by Conekta
    $address_value = $order_wrapper->commerce_customer_billing->commerce_customer_address->value();

    if (empty($address_value['name_line']) && empty($address_value['first_name'])) {
      $valid_billing_address = FALSE;
    }

    foreach (array('thoroughfare', 'locality', 'postal_code', 'country') as $address_key) {
      if (empty($address_value[$address_key])) {
        $valid_billing_address = FALSE;
      }
    }
  }

  // Without a valid billing address, display and log the error messages and
  // prevent the payment attempt.
  if (!$valid_billing_address) {
    // Display a general error to the customer if we can't find the address.
    drupal_set_message(t('We cannot process your credit card payment without a valid billing address.'), 'error');

    // Provide a more descriptive error message in the failed transaction and
    // the watchdog.
    $transaction = commerce_payment_transaction_new('paypal_wpp', $order->order_id);
    $transaction->instance_id = $payment_method['instance_id'];
    $transaction->amount = $order_total['amount'];
    $transaction->currency_code = $order_total['currency'];
    $transaction->payload[REQUEST_TIME] = array();
    $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
    $transaction->message = t('The customer must be able to supply a billing address through the default address field of the core billing information customer profile to pay via Conekta.');
    commerce_payment_transaction_save($transaction);

    watchdog('conekta_cc', 'A Conekta credit card transaction failed because the order did not have a value for the default billing address field.', NULL, WATCHDOG_ERROR);

    return FALSE;
  }

  // Use the corresponding key depending if environment is test or live
  if ($payment_method['settings']['server'] == 'sandbox') {
    $private_key = $payment_method['settings']['api_key_private_test'];
    $livemode = FALSE;
    $card = 'tok_test_visa_4242';
  }
  else {
    $private_key = $payment_method['settings']['api_key_private_live'];
    $livemode = TRUE;
    $card = $token;
  }

  // Load the conekta library and fail if was not correctly load
  if (($library = libraries_load('conekta-php')) && empty($library['loaded'])) {
    drupal_set_message(t('We encountered an internal error processing your payment, please try later.'), 'error');
    watchdog('conekta_cc', 'Conekta PHP library was not loaded correctly. <pre>@library</pre>', array('@library' => $library), WATCHDOG_ERROR);
    return FALSE;
  }

  Conekta::setApiKey($private_key);
  // TODO: Ask to Conekta the available locale options.
  Conekta::setLocale($language->language);
  // Prepare the request details array
  $line_items = commerce_conekta_order_line_items_list($order_wrapper);
  $billing_address = $order_wrapper->commerce_customer_billing->commerce_customer_address->value();

  $details = array(
    "email" => $order->mail,
    "name" => $billing_address['name_line'],
    // For the moment do not pass line items due it causes: Gateway Error: Hubo
    // un error del lado del sistema y los ingenieros de Conekta han sido notificados.
    // when the charge is made in MXN currency.
    //"line_items"  => $line_items,
    "billing_address"  => array(
      "street1" => $billing_address['thoroughfare'],
      "street2" => $billing_address['premise'],
      "zip" => $billing_address['postal_code'],
      "city" => $billing_address['locality'],
      "state" => $billing_address['administrative_area'],
      "country" => $billing_address['country'],
      //"phone" => $data['card']['phone'],
    )
  );

  $params = array(
    'livemode' => $livemode,
    'amount' => $amount,
    // Conekta only supports USD and MXN currencies
    'currency' => $currency_code,
    'card' => $card,
    'reference_id' => $order->order_id,
    'description' => 'Purchase for order #'. $order->order_id,
    'details' => $details,
  );

  try {
    $response = Conekta_Charge::create($params);
    $transaction_id = $response->id;
    // Convert to simple array for store on the transaction record
    $response = json_decode($response->__toString());
  }
  catch(Conekta_Error $e) {
    $description = $e->message_to_purchaser;
    watchdog('conekta_cc', 'Gateway Error: @description - <pre> @params </pre>', array('@description' => $description, '@params' => print_r($params, 1)), WATCHDOG_ERROR);
  }

  // Prepare a transaction object to log the API response.
  $transaction = commerce_payment_transaction_new('conekta_cc', $order->order_id);
  $transaction->instance_id = $payment_method['instance_id'];
  $transaction->amount = $order_total['amount'];
  $transaction->currency_code = $order_total['currency_code'];
  $transaction->payload[REQUEST_TIME] = $response;
  $transaction->remote_id = $response->id;

  // Build a meaningful response message.
  $message = array();

  // status: paid, pending, declined
  // In case that is pending could be that require a bank payment confirmation
  // so we should implement webhook to know when is confirmed or cancel order if
  // no response is received within next 48 hours.
  // Set the remote ID and transaction status based on the acknowledgment code.
  if ($response->status == 'paid') {
    $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
    $transaction->remote_status = $response->status;
    $message[] = t('Transaction success @id', array('@id' => $response->id));
  }
  else if ($response->status == 'pending') {
    $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
    $transaction->remote_status = $response->status;
    $message[] = t('Transaction pending @id', array('@id' => $response->id));
  }
  else {
    $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;

    // Build the transaction failed error depending if we got an exception
    if (isset($response)) {
      $message[] = t('Transaction failed @code - @error', array('@code' => $response->failure_code, '@error' => $response->failure_message));
    }
    else {
      $message[] = t('Transaction failed @error', array('@error' => $description));
    }
  }

  // Set the final message.
  $transaction->message = implode('<br />', $message);

  // Save the transaction information.
  commerce_payment_transaction_save($transaction);

  // If the payment failed, display an error and rebuild the form.
  if (!isset($response) || !in_array($response->status, array('paid', 'pending'))) {
    drupal_set_message(t('We encountered an error processing your payment. Please verify your credit card details or try a different card.'), 'error');
    return FALSE;
  }
}
