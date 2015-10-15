<?php

/**
 * @file
 * Callback methods for Realex Payments Remote integration.
 */

/**
 * Implements hook_commerce_payment_method_settings_form().
 */
function commerce_realex_remote_commerce_payment_method_settings_form($settings = NULL) {
  $form = array();

  $form['merchant_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Realex Payments Merchant ID'),
    '#default_value' => isset($settings['merchant_id']) ? $settings['merchant_id'] : '',
    '#size' => 16,
    '#required' => TRUE,
  );

  $form['account_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Realex Payments Account ID'),
    '#default_value' => isset($settings['account_id']) ? $settings['account_id'] : '',
    '#size' => 16,
    '#required' => FALSE,
  );

  $form['shared_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Shared secret'),
    '#default_value' => isset($settings['shared_secret']) ?  $settings['shared_secret'] : '',
    '#size' => 16,
    '#required' => TRUE,
  );

  $form['remote_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Realex Payments Remote XML URL'),
    '#description' => t('The Realex Payments URL to submit the order to.'),
    '#default_value' => isset($settings['remote_url']) ?  $settings['remote_url'] : 'https://epage.payandshop.com/epage-remote.cgi',
    '#size' => 50,
    '#required' => TRUE,
  );

  if (module_exists('commerce_3d_secure')) {
    $form['3dsecure'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use 3D Secure'),
      '#default_value' => isset($settings['3dsecure']) ? $settings['3dsecure'] : 0,
    );

    $form['3dsecure_remote_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Realex Payments 3D Secure XML URL'),
      '#description' => t('The Realex Payments URL to submit the order to.'),
      '#default_value' => isset($settings['3dsecure_remote_url']) ? $settings['3dsecure_remote_url'] : 'https://epage.payandshop.com/epage-3dsecure.cgi',
      '#size' => 50,
      '#required' => TRUE,
      '#states' => array(
        'visible' => array(
          ':input[name="parameter[payment_method][settings][payment_method][settings][3dsecure]"]' => array('checked' => TRUE),
        ),
      ),
    );

    $form['3dsecure_require_liability_shift'] = array(
      '#type' => 'checkbox',
      '#title' => t('Require liability shift where possible'),
      '#description' => t('If checked, transactions will only be attempted to be authorised if there is a liability shift away from the merchant. Note, not all card types, e.g. Laser, Amex, corporate VISA cards, support 3D Secure and for these the transaction will continue even though there is no liability shift available.'),
      '#default_value' => isset($settings['3dsecure_require_liability_shift']) ? $settings['3dsecure_require_liability_shift'] : 0,
      '#states' => array(
        'visible' => array(
          ':input[name="parameter[payment_method][settings][payment_method][settings][3dsecure]"]' => array('checked' => TRUE),
        ),
      ),
    );
  }
  else {
    $form['3dsecure'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use 3D Secure'),
      '#default_value' => 0,
      '#attributes' => array('disabled' => 'disabled'),
      '#description' => t('Enable the Commerce 3D Secure module if you wish to enable 3D Secure transactions for Realex Payments.'),
    );
  }

  $form['auto_settle'] = array(
    '#type' => 'select',
    '#title' => t('Payment action'),
    '#options' => array(1 => t('Authorize and Capture'), 0 => t('Authorize only')),
    '#default_value' => isset($settings['auto_settle']) ?  $settings['auto_settle'] : 1,
    '#required' => TRUE,
  );

  $form['varref'] = array(
    '#type' => 'textfield',
    '#title' => t('Variable reference'),
    '#description' => t('Any reference you also would like to assign to the customer. This can allow checking, using realscore, of previous transactions by this customer.'),
    '#default_value' => isset($settings['varref']) ?  $settings['varref'] : '',
    '#size' => 50,
    '#required' => FALSE,
  );


  /*
  $form['dcc_provider'] = array(
    '#type' => 'select',
    '#title' => t('Direct Currency Conversion (DCC)'),
    '#options' => array(
      'none' => 'None',
      //'fintrax' => 'Fintrax', Not available yet
      'fexco' => 'Fexco',
      'euroConex' => 'Elavon',
     ),
    '#description' => t('Should DCC be available to the user? You must have this preconfigured on your Realex Payments sub-account in advance.'),
    '#default_value' => isset($settings['dcc_provider']) ? $settings['dcc_provider'] : 'none',
  );
  */

  $cctypes = _commerce_realex_supported_card_types();
  $form['card_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Available card types'),
    '#options' => $cctypes,
    '#default_value' => isset($settings['card_types']) ? $settings['card_types'] : array(),
    '#required' => TRUE,
  );

  $form['cctype_subaccounts_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use separate accounts for each card type'),
    '#description' => t('Some configurations require different accounts to be used for different card types.'),
    '#default_value' => isset($settings['cctype_subaccounts_enabled']) ? $settings['cctype_subaccounts_enabled'] : array(),
  );

  $form['cctype_accounts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Realex Payments card type account settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="parameter[payment_method][settings][payment_method][settings][cctype_subaccounts_enabled]"]' => array('checked' => TRUE),
      ),
    ),
  );
  foreach ($cctypes as $cctype => $label) {
    $form['cctype_accounts'][$cctype] = array(
      '#type' => 'textfield',
      '#title' => t('@cctype account', array('@cctype' => $label)),
      '#default_value' => !empty($settings['cctype_accounts'][$cctype]) ? $settings['cctype_accounts'][$cctype] : '',
      '#required' => FALSE,
    );
  }

  return $form;
}

/**
 * Implements hook_commerce_payment_method_submit_form().
 *
 * Set allowed credit card types.
 */
function commerce_realex_remote_commerce_payment_method_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');
  $supported_card_types = _commerce_realex_supported_card_types();
  $available_card_types = $payment_method['settings']['card_types'];
  $card_types = array_intersect(array_keys($supported_card_types), $available_card_types);

  // Set which cards are available to the user.
  $credit_card_settings = array(
    'owner' => '',
    'type' => $card_types,
    'number' => '',
    'start_date' => NULL,
    'issue' => '',
    'code' => '',
  );

  // Get credit card fields.
  $form = commerce_payment_credit_card_form($credit_card_settings);

  return $form;
}

/**
 * Implements hook_commerce_payment_method_submit_form_validate().
 *
 * Validate the credit card fields.
 */
function commerce_realex_remote_commerce_payment_method_submit_form_validate($payment_method, $pane_form, $pane_values, $order, $form_parents = array()) {
  module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');

  $settings = array(
    'form_parents' => array_merge($form_parents, array('credit_card')),
  );
  $prefix = implode('][', $settings['form_parents']) . '][';

  $card_type = $pane_values['credit_card']['type'];
  if ($card_type == 'switch') {
    if (empty($pane_values['credit_card']['issue'])) {
      form_set_error($prefix . 'issue', t('Issue number field is required.'));
      return FALSE;
    }
  }
  elseif ($card_type != 'laser') {
    if (empty($pane_values['credit_card']['code'])) {
      form_set_error($prefix . 'code', t('Security code field is required.'));
      return FALSE;
    }
  }
  if (!commerce_payment_credit_card_validate($pane_values['credit_card'], $settings)) {
    return FALSE;
  }
}

/**
 * Implements hook_commerce_payment_method_submit_form_submit().
 */
function commerce_realex_remote_commerce_payment_method_submit_form_submit($payment_method, $pane_form, $pane_values, &$order, $charge) {
  $start_date = NULL;
  if (!empty($pane_values['credit_card']['start_month'])) {
    $start_date = sprintf("%02d%02d", $pane_values['credit_card']['start_month'], substr($pane_values['credit_card']['start_year'], -2));
  }
  $expiry_date = sprintf("%02d%02d", $pane_values['credit_card']['exp_month'], substr($pane_values['credit_card']['exp_year'], -2));

  $request = array();
  $request['timestamp'] = strftime('%Y%m%d%H%M%S');
  mt_srand((double) microtime() * 1000000);

  $request['order_number'] = $order->order_number;
  $request['orderid'] = $order->order_id . '-' . $request['timestamp'];
  $request['merchantid'] = check_plain($payment_method['settings']['merchant_id']);
  $request['account'] = check_plain($payment_method['settings']['account_id']);
  $request['secret'] = check_plain($payment_method['settings']['shared_secret']);
  $request['auto_settle'] = $payment_method['settings']['auto_settle'];
  if (!empty($payment_method['settings']['varref'])) {
    $request['varref'] = $payment_method['settings']['varref'];
  }
  // Allow other modules to alter the variable reference - used to tag orders.
  drupal_alter('commerce_realex_varref', $request['varref'], $order);
  $request['varref'] = check_plain($request['varref']);

  $request['3dsecure'] = (module_exists('commerce_3d_secure') ?  $payment_method['settings']['3dsecure']: 0);

  //$request['dcc_provider'] = $payment_method['settings']['dcc_provider'];

  $request['chname'] = check_plain($pane_values['credit_card']['owner']);
  $request['ccnumber'] = check_plain($pane_values['credit_card']['number']);
  $request['cctype'] = _commerce_realex_get_card_type($pane_values['credit_card']['type'], $pane_values['credit_card']['number']);
  $request['cvn'] = isset($pane_values['credit_card']['code']) ? check_plain($pane_values['credit_card']['code']) : NULL;
  $request['issueno'] = isset($pane_values['credit_card']['issue']) ? check_plain($pane_values['credit_card']['issue']) : '';
  $request['start_date'] = check_plain($start_date);
  $request['expiry_date'] = check_plain($expiry_date);

  // Sub-accounts can differ by card type
  if ($payment_method['settings']['cctype_subaccounts_enabled']) {
    $subaccount = $payment_method['settings']['cctype_accounts'][$pane_values['credit_card']['type']];
    $request['account'] = !empty($subaccount) ? check_plain($subaccount) : $request['account'];
  }

  // Load the customer billing profile for the order.
  if (!empty($order->commerce_customer_billing)) {
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
    $profile = $order_wrapper->commerce_customer_billing->value();
    $profile_wrapper = entity_metadata_wrapper('commerce_customer_profile', $profile);
    $address = $profile_wrapper->commerce_customer_address->value();
    $request['billing']['customer_id'] = $profile->profile_id;
    if (isset($address['name_line'])) {
      $request['billing']['customer_name'] = check_plain($address['name_line']);
    }
    elseif (isset($address['first_name'])) {
      $request['billing']['customer_name'] = check_plain($address['first_name'] . ' ' .  $address['last_name']);
    }
    $request['billing']['street_one'] = isset($address['thoroughfare']) ? check_plain($address['thoroughfare']) : '';
    $request['billing']['postcode'] = isset($address['postal_code']) ? check_plain($address['postal_code']) : '';
    $zip_numbers = preg_replace('/[^0-9]/', '', $request['billing']['postcode']);
    $street_numbers = preg_replace('/[^0-9]/', '', $request['billing']['street_one']);
    $zip_postal_code = substr($zip_numbers, 0, 5) . '|' . substr($street_numbers, 0, 5);
    $request['billing']['zip|postal_code'] = ($zip_postal_code == '|') ? '' : $zip_postal_code;
    $request['billing']['city'] = isset($address['locality']) ? check_plain($address['locality']) : '';
    $request['billing']['region'] = isset($address['administrative_area']) ? check_plain($address['administrative_area']) : '';
    $request['billing']['country'] = isset($address['country']) ? check_plain($address['country']) : '';
  }

  // Load the customer shipping profile for the order.
  if (!empty($order->commerce_customer_shipping)) {
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
    $profile = $order_wrapper->commerce_customer_shipping->value();
    $profile_wrapper = entity_metadata_wrapper('commerce_customer_profile', $profile);
    $address = $profile_wrapper->commerce_customer_address->value();
    $request['shipping']['customer_id'] = $profile->profile_id;
    if (isset($address['name_line'])) {
      $request['shipping']['customer_name'] = check_plain($address['name_line']);
    }
    elseif (isset($address['first_name'])) {
      $request['shipping']['customer_name'] = check_plain($address['first_name'] . ' ' .  $address['last_name']);
    }
    $request['shipping']['street_one'] = isset($address['thoroughfare']) ? check_plain($address['thoroughfare']) : '';
    $request['shipping']['postcode'] = isset($address['postal_code']) ? check_plain($address['postal_code']) : '';
    $zip_numbers = preg_replace('/[^0-9]/', '', $request['shipping']['postcode']);
    $street_numbers = preg_replace('/[^0-9]/', '', $request['shipping']['street_one']);
    $zip_postal_code = substr($zip_numbers, 0, 5) . '|' . substr($street_numbers, 0, 5);
    $request['shipping']['zip|postal_code'] = ($zip_postal_code == '|') ? '' : $zip_postal_code;
    $request['shipping']['city'] = isset($address['locality']) ? check_plain($address['locality']) : '';
    $request['shipping']['region'] = isset($address['administrative_area']) ? check_plain($address['administrative_area']) : '';
    $request['shipping']['country'] = isset($address['country']) ? check_plain($address['country']) : '';
  }
  $request['customer_id'] = isset($request['billing']['customer_id']) ? $request['billing']['customer_id'] : (!empty($request['shipping']['customer_id']) ? $request['shipping']['customer_id'] : 0);
  $request['amount'] = $charge['amount'];
  $request['currency'] = $charge['currency_code'];
  $request['ip_address'] = ip_address();

  // Get products purchased.
  $products = array();
  $comments = array();
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  foreach ($order_wrapper->commerce_line_items as $delta => $line_item_wrapper) {
    $line_item = $line_item_wrapper->value();
    // If the line item is a product line item...
    if (in_array($line_item->type, commerce_product_line_item_types())) {
      $product = $line_item_wrapper->commerce_product->value();
      $products[] = $product->product_id;
      $comments[] = sprintf("%dx %s",$line_item->quantity, check_plain($product->title));
    }
  }

  $request['products'] = implode(',', $products);
  $request['comments'][0] = t('Products purchased:') . ' ' . implode(', ', $comments);
  $request['comments'][1] = _commerce_realex_get_module_versions();
  $request['log_message'] = '';

  // Build sha1 hash.
  $request['sha1hash'] = _commerce_realex_build_request_sha1hash($request);

  // 3D Secure transaction.
  module_load_include('inc', 'commerce_realex', 'includes/commerce_realex_remote_3dsecure');
  if ($request['3dsecure'] && _commerce_realex_is_3dsecure_supported_card($pane_values['credit_card']['type'])) {
    commerce_realex_remote_do_3dsecure_verify_enrolled_request($payment_method, $request, $order);
  }

  // DCC enabled, so get offer rather than processing payment.
  //elseif ($request['dcc_provider'] != 'none') {
   // $dcc_message = commerce_realex_remote_get_dcc_offer($payment_method, $pane_values, $order, $charge);
  //}

  // Standard authorisation.
  else {
    if ($request['3dsecure']) {
      watchdog('commerce_realex', 'Card does not support 3D Secure, no liability shift (orderid: @order_id)', array('@order_id' => $order->order_id));
      $request['3dsecure'] = FALSE;
    }
    commerce_realex_remote_do_auth_request($payment_method, $request, $order);
  }

  return;
}

/**
 * Do a payment authorization request.
 */
function commerce_realex_remote_do_auth_request($payment_method, $request, &$order) {
  // Send the request.
  $type = 'auth';
  $url = $payment_method['settings']['remote_url'];
  $payment_method['instance_id'] = $order->data['payment_method'];
  $xml = _commerce_realex_remote_build_xml_request($request, $type);
  $response = _commerce_realex_send_request($url, $xml);

  // Verify response.
  $success = _commerce_realex_remote_verify_response($type, $request, $response, $order, $payment_method);

  // Save the transaction.
  $log = isset($request['log_message']) ? $request['log_message'] : '';
  if (!empty($request['eci'])) {
    $eci_scenario = commerce_realex_remote_3dsecure_eci_lookup($request['eci']);
    $log = 'ECI sent: ' . $eci_scenario . ' (' . $request['eci'] . ')<br />' . $log;
  }
  $transaction = commerce_realex_remote_transaction_save($type, $payment_method, $order, $response, $log);
  unset($request['log_message']);

  // Successful payment.
  if ($success) {
    //$order->data['commerce_realex_remote'] = $pane_values;
    $order->data['commerce_realex_remote']['transaction'] = $transaction;
    //commerce_checkout_complete($order);
    $order = commerce_order_status_update($order, 'checkout_payment');
    $redirect = commerce_checkout_order_uri($order);
    if (function_exists('commerce_3d_secure_clear_iframe')) {
      commerce_3d_secure_clear_iframe($redirect);
    }
  }

  // Unsuccessful payments.
  else {
    // Payment failure.
    drupal_set_message(t('We were unable to process your credit card payment; reason provided by bank: %message. If the problem persists, contact us to complete your order.', array('%message' => $response['message'])), 'warning');
    watchdog('commerce_realex', 'Payment failed: %message', array('%message' => $response['message']), WATCHDOG_WARNING);

    if (empty($_SESSION['realex_failures'])) {
      $_SESSION['realex_failures'] = 0;
    }
    $_SESSION['realex_failures']++;

    // 3D secure failure.
    if (isset($request['cavv'])) {
      commerce_realex_remote_3dsecure_previous_page($order);
      $redirect = commerce_checkout_order_uri($order);
      commerce_3d_secure_clear_iframe($redirect);
    }
    else {
      drupal_goto('checkout');
    }
  }
}

/**
 * Contact Realex Payments to get a DCC offer.
 */
function commerce_realex_remote_get_dcc_offer($payment_method, $pane_values, &$order, $charge) {
  $start_date = NULL;
  if (!empty($pane_values['credit_card']['start_month'])) {
    $start_date = sprintf("%02d%02d", $pane_values['credit_card']['start_month'], substr($pane_values['credit_card']['start_year'], -2));
  }
  $expiry_date = sprintf("%02d%02d", $pane_values['credit_card']['exp_month'], substr($pane_values['credit_card']['exp_year'], -2));

  $request = array();
  $request['timestamp'] = strftime('%Y%m%d%H%M%S');
  mt_srand((double) microtime() * 1000000);

  // TODO - review.
  // You can use any alphanumeric combination for the orderid.
  // Although each transaction must have a unique orderid.
/*
  $orderid = $order->order_id;
  if (isset($_SESSION['realex_failures'])) {
    $orderid = $orderid . '-' . $_SESSION['realex_failures'];
  }
*/

  $request['order_number'] = $order->order_number;
  $request['orderid'] = $order->order_id . '-' . $request['timestamp'];
  $request['merchantid'] = check_plain($payment_method['settings']['merchant_id']);
  $request['account'] = check_plain($payment_method['settings']['account_id']);
  $request['secret'] = check_plain($payment_method['settings']['shared_secret']);
  $request['auto_settle'] = $payment_method['settings']['auto_settle'];
  if (!empty($payment_method['settings']['varref'])) {
    $request['varref'] = $payment_method['settings']['varref'];
  }
  // Allow other modules to alter the variable reference - used to tag orders.
  drupal_alter('commerce_realex_varref', $request['varref'], $order);
  $request['varref'] = check_plain($request['varref']);

  $request['dcc_provider'] = check_plain($payment_method['settings']['dcc_provider']);

  $request['chname'] = check_plain($pane_values['credit_card']['owner']);
  $request['ccnumber'] = check_plain($pane_values['credit_card']['number']);
  $request['cctype'] = _commerce_realex_get_card_type($pane_values['credit_card']['type'], $request['ccnumber']);
  $request['cvn'] = isset($pane_values['credit_card']['code']) ? check_plain($pane_values['credit_card']['code']) : NULL;
  $request['issueno'] = isset($pane_values['credit_card']['issue']) ? check_plain($pane_values['credit_card']['issue']) : '';
  $request['start_date'] = check_plain($start_date);
  $request['expiry_date'] = check_plain($expiry_date);
  $request['amount'] = $charge['amount'];
  $request['currency'] = $charge['currency_code'];

  // Build sha1 hash.
  $request['sha1hash'] = _commerce_realex_build_request_sha1hash($request);

  // Send the request.
  $url = $payment_method['settings']['remote_url'];
  $type = 'dccrate';
  $xml = _commerce_realex_remote_build_xml_request($request, $type);
  $response = _commerce_realex_send_request($url, $xml);

  // Verify response.
  $success = _commerce_realex_remote_verify_response($type, $request, $response, $order, $payment_method);

  // Got DCC offer, set up variables.
  if ($success) {
    $cardholdercurrency = $response['cardholdercurrency'];
    $cardholderamount = $response['cardholderamount'];
    $cardholderamountdisplay = $cardholderamount / 100;
    $cardholderrate = $response['cardholderrate'];
    $merchantcurrency = $response['merchantcurrency'];
    $merchantamount = $response['merchantamount'];

    // Stash these in the session for later use.
    $_SESSION['cardholdercurrency'] = $cardholdercurrency;
    $_SESSION['cardholderamount'] = $cardholderamount;
    $_SESSION['cardholderrate'] = $cardholderrate;
    $_SESSION['has_dcc_offer'] = TRUE;

    // Save them to the order too.
    $order->data['cardholdercurrency'] = $cardholdercurrency;
    $order->data['cardholderamount'] = $cardholderamount;
    $order->data['cardholderrate'] = $cardholderrate;
    $order->data['has_dcc_offer'] = TRUE;

    $currency_names = array(
      'AUD' => t('Austalian Dollars'),
      'GBP' => t('Pounds Sterling'),
      'USD' => t('US Dollars'),
      'CHF' => t('Swiss Francs'),
      'EUR' => t('Euro'),
    );

    // Use human friendly names if we have them.
    if ($currency_names[$cardholdercurrency]) {
      $cardholdercurrency = $currency_names[$cardholdercurrency];
    }
    if ($currency_names[$merchantcurrency]) {
      $merchantcurrency = $currency_names[$merchantcurrency];
    }

    $message = t('We notice that your credit card is in @currency.', array('@currency' => $cardholdercurrency)) . '<br />';
    $message .= t('For your convenience you can opt to have this transaction charged as <br />@cardholderamountdisplay @cardholdercurrency (at an exchange rate of 1.00 @merchantcurrency = @cardholderrate @cardholdercurrency)', array(
      '@cardholderamountdisplay' => $cardholderamountdisplay,
      '@cardholdercurrency' => $cardholdercurrency,
      '@merchantcurrency' => $merchantcurrency,
      '@cardholderrate' => $cardholderrate,
      '@cardholdercurrency' => $cardholdercurrency,
    ));
    $order->data['commerce_realex_dcc_message'] = $message;
  }

  else {
    watchdog('commerce_realex', 'DCC offer failed: %message', array('%message' => $response['message']), WATCHDOG_WARNING);
  }

  return $success;
}

/**
 * Process the Realex Payments response.
 *
 * @param $type
 *   Message response type.
 * @param $response
 *   Parsed response as an array.
 * @param $request
 *   Transaction details array.
 * @param $order
 *   Commerce order object.
 * @param $payment_method
 *   Payment method array.
 */
function _commerce_realex_remote_verify_response($type, $request, &$response, $order, $payment_method) {
  if (!isset($response['result'])) {
    watchdog('commerce_realex', 'No results code, something has gone wrong with the transaction.', array(), WATCHDOG_ERROR);
    drupal_goto('checkout/' . $order->order_id . '/payment');
  }

  $merchantid = $request['merchantid'];
  $secret = $request['secret'];
  $orderid = $request['orderid'];
  $timestamp = $response['timestamp'];
  $resultcode = $response['result'];
  $message = $response['message'];
  if ($response['result'] != '00') {
    return FALSE;
  }
  $pasref = isset($response['pasref']) ? $response['pasref'] : '';
  $authcode = isset($response['authcode']) ? $response['authcode'] : ''; // Needed for Fexco DCC.

  // This digital signature should correspond to the one Realex Payments returns
  // and can therefore be used to verify the message Realex Payments sends back.
  $tmp = "$timestamp.$merchantid.$orderid.$resultcode.$message.$pasref.$authcode";
  $sha1hash = sha1($tmp);
  $tmp = "$sha1hash.$secret";
  $sha1hash = sha1($tmp);

  $success = FALSE;

  // Check to see if hashes match or not
  if ($sha1hash != $response['sha1hash']) {
    watchdog('commerce_realex', 'Hash match failed for order @order_id.', array('@order_id' => $response['orderid']));
    drupal_set_message(t("Hashes don't match - response not authenticated!"), 'error');
    $response['user_message'] = 'Bank connection error, please try again later';
  }
  else {
    $success = TRUE;
  }

  // Unlikely, but just in case they can ever decrypt it to get card number.
  unset($response['sha1hash']);

  return $success;
}

/**
 * Generate xml
 *
 * @param request
 *     An array with all the transaction data.
 */
function _commerce_realex_remote_build_xml_request($request, $type) {
  module_load_include('inc', 'commerce_realex', 'includes/commerce_realex_remote_3dsecure');
  switch ($type) {
    case 'dccrate':
      return _commerce_realex_remote_build_xml_request_dccrate($request);
    case '3ds-verifysig':
      return _commerce_realex_remote_build_xml_request_3dsecure_verifysig($request);
    case '3ds-verifyenrolled':
      return _commerce_realex_remote_build_xml_request_3dsecure_verifyenrolled($request);
    case 'auth':
    default:
      return _commerce_realex_remote_build_xml_request_auth($request);
  }
}

/**
 * Generate an "auth" xml request.
 */
function _commerce_realex_remote_build_xml_request_auth($request) {
  $request['ip_address'] = isset($request['ip_address']) ? $request['ip_address'] : ip_address();
  $xml = "<request type='auth' timestamp='" . $request['timestamp'] . "'>
    <merchantid>" . $request['merchantid'] . "</merchantid>
    <account>" . $request['account'] . "</account>
    <orderid>" . $request['orderid'] . "</orderid>
    <amount currency='" . $request['currency'] . "'>" . $request['amount'] . "</amount>
    <card>
      <number>" . $request['ccnumber'] . "</number>
      <expdate>" . $request['expiry_date'] . "</expdate>
      <type>" . $request['cctype'] . "</type>
      <chname>" . $request['chname'] . "</chname>";
  if (isset($request['issueno'])) {
    $xml .= "
      <issueno>" . $request['issueno'] . "</issueno>";
  }
  $xml .= "
    </card>";
  if (isset($request['cvn'])) {
    $xml .="
    <cvn>
      <number>" . $request['cvn'] . "</number>
      <presind>1</presind>
    </cvn>";
  }
  if (isset($request['dcc'])) {
    $xml .="
    <dccinfo>
      <ccp>" . $request['dcc_provider'] . "</ccp>
      <type>1</type>
      <rate>" . $request['cardholderrate'] . "</rate>
      <ratetype>S</ratetype>
      <amount currency='" . $request['cardholdercurrency'] . "'>" . $request['cardholderamount'] . "</amount>
    </dccinfo>";
  }
  if (!empty($request['comments']) && is_array($request['comments'])) {
    $xml .= "
    <comments>
      <comment id='1'>" . $request['comments'][0] . "</comment>";
    if (count($request['comments']) > 1) {
      $xml .= "
      <comment id='2'>" . $request['comments'][1] . "</comment>";
    }
    $xml .= "
    </comments>";
  }
  if (!empty($request['3dsecure']) && module_exists('commerce_3d_secure')) {
    $xml .="
    <mpi>";
    if (isset($request['cavv'])) {
      $xml .= "
      <cavv>" . $request['cavv'] . "</cavv>";
    }
    if (isset($request['xid'])) {
      $xml .= "
      <xid>" . $request['xid'] . "</xid>";
    }
    if (isset($request['eci'])) {
      $xml .= "
      <eci>" . $request['eci'] . "</eci>";
    }
    $xml .="
    </mpi>";
  }
  $xml .= "
    <autosettle flag='" . $request['auto_settle'] . "'/>
    <sha1hash>" . $request['sha1hash'] . "</sha1hash>
    <tssinfo>";
  $xml .=  "
      <custipaddress>" . $request['ip_address'] . "</custipaddress>";
  if (!empty($request['billing'])) {
    $xml .= "
      <address type=\"billing\">
        <country>" . $request['billing']['country'] . "</country>
        <code>" . $request['billing']['zip|postal_code'] . "</code>
      </address>";
  }
  if (!empty($request['shipping'])) {
    $xml .= "
      <address type=\"shipping\">
        <country>" . $request['shipping']['country'] . "</country>
        <code>" . $request['shipping']['zip|postal_code'] . "</code>
      </address>";
  }
  if (!empty($request['customer_id'])) {
      $xml .= "
      <custnum>" . $request['customer_id'] . "</custnum>";
  }
  if (!empty($request['products'])) {
    $xml .= "
      <prodid>" . $request['products'] . "</prodid>";
  }
  if (!empty($request['varref'])) {
    $xml .= "
      <varref>" . $request['varref'] . "</varref>";
  }
  $xml .="
    </tssinfo>
  </request>";

  return $xml;
}

/**
 * Generate an "dccrate" xml request.
 */
function _commerce_realex_remote_build_xml_request_dccrate($request) {
  $xml = "<request type='dccrate' timestamp='" . $request['timestamp'] . "'>
    <merchantid>" . $request['merchantid'] . "</merchantid>
    <account>" . $request['account'] . "</account>
    <orderid>" . $request['orderid'] . "</orderid>
    <amount currency='" . $request['currency'] . "'>" . $request['amount'] . "</amount>
    <card>
      <number>" . $request['ccnumber'] . "</number>
      <expdate>" . $request['expiry_date'] . "</expdate>
      <type>" . $request['cctype'] . "</type>
      <chname>" . $request['chname'] . "</chname>
      <issueno>" . $request['issueno'] . "</issueno>
    </card>
    <autosettle flag='" . $request['auto_settle'] . "'/>
    <dccinfo>
      <ccp>" . $request['dcc_provider'] . "</ccp>
      <type>1</type>
    </dccinfo>
    <sha1hash>" . $request['sha1hash'] . "</sha1hash>
  </request>";

  return $xml;
}


/**
 * Save the transaction and update the order.
 *
 * @param $payment_method
 *   Payment method array.
 * @param $order
 *   Commerce order object.
 * @param $response
 *   Response data.
 * @param $log
 *   Optional extra log message.
 */
function commerce_realex_remote_transaction_save($transaction_type, $payment_method, $order, $response, $log = '') {
  $transaction = commerce_payment_transaction_new('commerce_realex_remote', $order->order_id);
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $commerce_order_total = $order_wrapper->commerce_order_total->value();
  $transaction->amount = $commerce_order_total['amount'];
  $transaction->currency_code = $commerce_order_total['currency_code'];
  $transaction->instance_id = empty($payment_method['instance_id']) ? NULL : $payment_method['instance_id'];
  $transaction->remote_id = isset($response['orderid']) ? $response['orderid'] : '';

  // Set the payment status.
  if ($response['result'] == '00') {
    if ($transaction_type == 'auth') {
      $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
    }
    else {
      $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
    }
  }
  else {
    $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
  }

  // Log the response variables.
  $transaction->message = 'Transaction type: @type<br />Status @resultcode, @message.<br />Timestamp: @timestamp<br />';
  $transaction->message_variables = array(
    '@type' => $transaction_type,
    '@resultcode' => isset($response['result']) ? $response['result'] : '',
    '@message' => isset($response['message']) ? $response['message'] : '',
    '@timestamp' => isset($response['timestamp']) ? $response['timestamp'] : '',
  );
  if (!empty($response['orderid'])) {
    $transaction->message .= 'Order ID: @orderid<br />';
    $transaction->message_variables['@orderid'] = $response['orderid'];
  }
  if (!empty($response['authcode'])) {
    $transaction->message .= 'Auth code: @authcode<br />';
    $transaction->message_variables['@authcode'] = $response['authcode'];
  }
  if (!empty($response['pasref'])) {
    $transaction->message .= 'Pasref: @pasref<br />';
    $transaction->message_variables['@pasref'] = $response['pasref'];
  }
  if (!empty($response['avsaddressresponse'])) {
    $transaction->message .= 'Address check: @address<br />';
    $transaction->message_variables['@address'] = $response['avsaddressresponse'];
  }
  if (!empty($response['avspostcoderesponse'])) {
    $transaction->message .= 'Postcode check: @postcode<br />';
    $transaction->message_variables['@postcode'] = $response['avspostcoderesponse'];
  }
  if (!empty($response['cvnresult'])) {
    $transaction->message .= 'CVN result: @cvn<br />';
    $transaction->message_variables['@cvn'] = $response['cvnresult'];
  }
  if (!empty($response['batchid'])) {
    $transaction->message .= 'Batch Id: @batchid<br />';
    $transaction->message_variables['@batchid'] = $response['batchid'];
  }
  if (!empty($response['tss_result'])) {
    $transaction->message .= 'Transaction suitability score: @tss<br />';
    $transaction->message_variables['@tss'] = $response['tss_result'];
  }
  $eci = !empty($response['eci']) ? $response['eci'] : (!empty($response['threedsecure_eci']) ? $response['threedsecure_eci'] : NULL);
  if (!empty($eci)) {
    $transaction->message .= 'ECI: @eci_scenario (@eci)<br />';
    $transaction->message_variables['@eci'] = $eci;
    $transaction->message_variables['@eci_scenario'] = commerce_realex_remote_3dsecure_eci_lookup($eci);
  }
  $xid = !empty($response['xid']) ? $response['xid'] : (!empty($response['threedsecure_xid']) ? $response['threedsecure_xid'] : NULL);
  if (!empty($xid)) {
    $transaction->message .= 'XID: @xid<br />';
    $transaction->message_variables['@xid'] = $xid;
  }
  $enrolled = !empty($response['enrolled']) ? $response['enrolled'] : NULL;
  if (!empty($enrolled)) {
    $transaction->message .= 'Enrolled: @enrolled<br />';
    $transaction->message_variables['@enrolled'] = $enrolled;
  }
  $cavv = !empty($response['cavv']) ? $response['cavv'] : (!empty($response['threedsecure_cavv']) ? $response['threedsecure_cavv'] : NULL);
  if (!empty($cavv)) {
    $transaction->message .= 'CAVV: @cavv<br />';
    $transaction->message_variables['@cavv'] = $cavv;
  }
  if (!empty($response['threedsecure_status'])) {
    $transaction->message .= '3D Secure Status: @threedsecure_status<br />';
    $transaction->message_variables['@threedsecure_status'] = $response['threedsecure_status'];
  }
  if (!empty($response['log'])) {
    $transaction->message .= '@response_log<br />';
    $transaction->message_variables['@response_log'] = $response['log'];
  }
  if (!empty($log)) {
    $transaction->message .= '@log<br />';
    $transaction->message_variables['@log'] = $log;
  }
  if (!empty($response['user_message'])) {
    $transaction->message .= 'Error displayed: @user_message<br />';
    $transaction->message_variables['@user_message'] = $response['user_message'];
  }

  commerce_payment_transaction_save($transaction);

  return $transaction;
}

