<?php

/**
 * @file
 * Callback methods for Realex Payments Redirect integration.
 */

/**
 * Payment method callback: settings form.
 */
function commerce_realex_redirect_commerce_payment_method_settings_form($settings = NULL) {
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
    '#default_value' => isset($settings['account_id']) ?  $settings['account_id'] : '',
    '#size' => 16,
    '#required' => TRUE,
  );

  $form['shared_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Shared secret'),
    '#default_value' => isset($settings['shared_secret']) ? $settings['shared_secret'] : '',
    '#size' => 16,
    '#required' => TRUE,
  );

  $form['test_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable test mode'),
    '#description' => t('If enabled, the transactions go to a sandbox Realex Payments server.'),
    '#default_value' => isset($settings['test_mode']) ? $settings['test_mode'] : 0,
  );

  $form['redirect_referring_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Referring URL'),
    '#description' => t('The absolute url to the page on your site from which
    the Realex Payments gateway will be referred by. You will need to give this url to Realex Payments when they set up your account.'),
    '#default_value' => isset($settings['redirect_referring_url']) ? $settings['redirect_referring_url'] : url('checkout/', array('absolute' => TRUE)) . '(.*)',
    '#required' => TRUE,
    '#disabled' => TRUE,
  );

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

  return $form;
}

/**
 * Implements hook_commerce_payment_method_submit_form().
 *
 * Add a message and CSS to the submission form.
 */
function commerce_realex_redirect_commerce_payment_method_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  $form['commmerce_realex_redirect_markup'] = array(
    '#markup' => '<span class="commerce-realex-redirect-information">' . t('You will be redirected to the Realex Payments Hosted Payment Page to complete your order.') . '</span>',
  );

  return $form;
}

/**
 * Payment method callback: redirect form, a wrapper around the module's general
 *   use function for building a realex form.
 */
function commerce_realex_redirect_redirect_form($form, &$form_state, $order, $payment_method) {
  // Return an error if the enabling action's settings haven't been configured.
  if (empty($payment_method['settings']['merchant_id'])) {
    drupal_set_message(t('Realex Payments is not configured for use. No Realex Payments merchant ID has been specified.'), 'error');
    return array();
  }

  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

  $request = array();
  $request['currency'] = $order_wrapper->commerce_order_total->currency_code->value();
  $request['amount'] = $order_wrapper->commerce_order_total->amount->value();

  // Get products purchased.
  $products = array();
  $comments = array();
  foreach ($order_wrapper->commerce_line_items as $delta => $line_item_wrapper) {
    $line_item = $line_item_wrapper->value();
    // If the line item is a product line item...
    if (in_array($line_item->type, commerce_product_line_item_types())) {
      $product = $line_item_wrapper->commerce_product->value();
      $products[] = $product->product_id;
      $comments[] = sprintf("%dx %s", $line_item->quantity, check_plain($product->title));
    }
  }

  $request['products'] = implode(',', $products);
  $request['comments'][0] = t('Products purchased:') . ' ' . implode(', ', $comments);

  $request['timestamp'] = strftime('%Y%m%d%H%M%S');
  mt_srand((double) microtime() * 1000000);

  $request['order_number'] = $order->order_number;
  $request['orderid'] = $order->order_id . '-' . $request['timestamp'];
  $request['merchantid'] = check_plain($payment_method['settings']['merchant_id']);
  $request['account'] = check_plain($payment_method['settings']['account_id']);
  $request['secret'] = check_plain($payment_method['settings']['shared_secret']);
  $request['auto_settle'] = $payment_method['settings']['auto_settle'];
  $request['varref'] = $payment_method['settings']['varref'];

  // Allow other modules to alter the variable reference - used to tag orders.
  drupal_alter('commerce_realex_varref', $request['varref'], $order);
  $request['varref'] = check_plain($request['varref']);

  $request['sha1hash'] = _commerce_realex_build_request_sha1hash($request, 'redirect-auth');

  // Build the data array that will be translated into hidden form values.
  $data = array(
    'MERCHANT_RESPONSE_URL' => url('commerce-realex/redirect/complete', array('absolute' => TRUE)),
    'MERCHANT_ID' => $request['merchantid'],
    'ORDER_ID' => $request['orderid'],
    'ACCOUNT' => $request['account'],
    'AMOUNT' => $request['amount'],
    'CURRENCY' => $request['currency'],
    'TIMESTAMP' => $request['timestamp'],
    'SHA1HASH' => $request['sha1hash'],
    'AUTO_SETTLE_FLAG' => $request['auto_settle'],
    'VAR_REF' => $request['varref'],
    'COMMERCE_ORDER_ID' => $order->order_id,
    'COMMERCE_ORDER_NUMBER' => $order->order_number,
    'RETURN_TSS' => 1,
  );

  if (!empty($request['products'])) {
    $data['PROD_ID'] = $request['products'];
  }
  if (!empty($request['comments']) && is_array($request['comments'])) {
    $data['COMMENT1'] = $request['comments'][0];
    if (strlen($data['COMMENT1']) > 255) {
      $data['COMMENT1'] = substr($data['COMMENT1'], 0, 252) . '...';
    }
  }
  $data['COMMENT2'] = _commerce_realex_get_module_versions();

  // Load the customer billing profile for the order.
  if (!empty($order->commerce_customer_billing)) {
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
    $profile = $order_wrapper->commerce_customer_billing->value();
    $profile_wrapper = entity_metadata_wrapper('commerce_customer_profile', $profile);
    $address = $profile_wrapper->commerce_customer_address->value();

    $data['CUST_NUM'] = $profile->profile_id;
    $street_one = isset($address['thoroughfare']) ? check_plain($address['thoroughfare']) : '';
    $postcode = isset($address['postal_code']) ? check_plain($address['postal_code']) : '';
    $zip_numbers = preg_replace('/[^0-9]/', '', $postcode);
    $street_numbers = preg_replace('/[^0-9]/', '', $street_one);
    $zip_postal_code = $zip_numbers . '|' . $street_numbers;
    $zip_postal_code = ($zip_postal_code == '|') ? '' : $zip_postal_code;
    $country = isset($address['country']) ? check_plain($address['country']) : '';

    $data['BILLING_CO'] = $country;
    $data['BILLING_CODE'] = $zip_postal_code;
  }

  // Load the customer shipping profile for the order.
  if (!empty($order->commerce_customer_shipping)) {
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
    $profile = $order_wrapper->commerce_customer_shipping->value();
    $profile_wrapper = entity_metadata_wrapper('commerce_customer_profile', $profile);
    $address = $profile_wrapper->commerce_customer_address->value();

    $data['CUST_NUM'] = !empty($data['CUST_NUM']) ? $data['CUST_NUM'] : $profile->profile_id;
    $street_one = isset($address['thoroughfare']) ? check_plain($address['thoroughfare']) : '';
    $postcode = isset($address['postal_code']) ? check_plain($address['postal_code']) : '';
    $zip_numbers = preg_replace('/[^0-9]/', '', $postcode);
    $street_numbers = preg_replace('/[^0-9]/', '', $street_one);
    $zip_postal_code = substr($zip_numbers, 0, 5) . '|' . substr($street_numbers, 0, 5);
    $zip_postal_code = ($zip_postal_code == '|') ? '' : $zip_postal_code;
    $country = isset($address['country']) ? check_plain($address['country']) : '';

    $data['SHIPPING_CO'] = $country;
    $data['SHIPPING_CODE'] = $zip_postal_code;
  }

  // Live url.
  $form['#action'] = 'https://hpp.realexpayments.com/pay';
  // Sandbox
  if (!empty($payment_method['settings']['test_mode'])) {
    $form['#action'] = 'https://hpp.sandbox.realexpayments.com/pay';
  }

  foreach ($data as $name => $value) {
    if (!empty($value)) {
      $form[$name] = array('#type' => 'hidden', '#value' => $value);
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Proceed to Realex Payments'),
  );

  return $form;
}


/**
 * Returns an array of all possible language codes.
 */
function commerce_realex_redirect_languages() {
  return drupal_map_assoc(array('AU', 'DE', 'FR', 'IT', 'GB', 'ES', 'US'));
}

/**
 * Returns an array of all possible currency codes.
 */
function commerce_realex_redirect_currencies() {
  return drupal_map_assoc(array('AUD', 'BRL', 'CAD', 'CHF', 'CZK', 'DKK', 'EUR', 'GBP', 'HKD', 'HUF', 'ILS', 'JPY', 'MXN', 'MYR', 'NOK', 'NZD', 'PHP', 'PLN', 'SEK', 'SGD', 'THB', 'TWD', 'USD'));
}

function commerce_realex_redirect_checkout_complete() {
  // Pull out the response data.
  $response = array();
  foreach ($_POST as $key => $value) {
    // Modify keys to lower case.
    $key = strtolower($key);
    $response[$key] = $value;
  }

  if ($response['commerce_order_id']) {
    $order = commerce_order_load($response['commerce_order_id']);

    $instance_id = $order->data['payment_method'];
    $payment_method = commerce_payment_method_instance_load($instance_id);

    $success = _commerce_realex_redirect_verify_response($response, $order, $payment_method);

    // Save the transaction.
    $log = isset($request['log_message']) ? $request['log_message'] : '';
    $transaction = commerce_realex_redirect_transaction_save($payment_method, $order, $response, $log);

    if ($success) {
      // Mark the order as complete.
      commerce_checkout_complete($order);

      $url = url('checkout/' . $order->order_id . '/complete', array('absolute' => TRUE));
      return _commerce_realex_redirect_back($url);
    }
  }

  // Invalid response or failure.
  $message = t('We were unable to process your credit card payment; reason provided by bank: %message. If the problem persists, contact us to complete your order.', array('%message' => $response['message']));
  $order = commerce_order_status_update($order, 'checkout_checkout', FALSE, TRUE, t('Realex Payments Redirect:') . ' ' . $message);
  drupal_set_message($message, 'warning');
  watchdog('commerce_realex', 'Payment failed: %message', array('%message' => $response['message']), WATCHDOG_WARNING);
  $uri = commerce_checkout_order_uri($order);
  $url = url($uri, array('absolute' => TRUE, 'query' => array('realex' => serialize($message))));
  return _commerce_realex_redirect_back($url);
}

/**
 * Redirect back to given url on the website.
 */
function _commerce_realex_redirect_back($url) {
  $output = '<style type="text/css">body {display: none;}</style>';
  $output .= '<script type="text/javascript"> window.location = "' . $url . '";</script>';
  print $output;
  return '';
}

/**
 * Process the Realex response.
 *
 * @param $response
 *   Parsed response as an array.
 * @param $order
 *   Commerce order object.
 * @param $payment_method
 *   Payment method array.
 */
function _commerce_realex_redirect_verify_response(&$response, $order, $payment_method) {
  if (!isset($response['result'])) {
    watchdog('commerce_realex', 'No results code, something has gone wrong with the transaction.', array(), WATCHDOG_ERROR);
    drupal_goto('checkout/' . $order->order_id . '/payment');
  }

  list($order_id, $timestamp) = explode('-', $response['order_id']);

  $merchantid = $payment_method['settings']['merchant_id'];
  $secret = $payment_method['settings']['shared_secret'];
  $orderid = $response['order_id'];
  $resultcode = $response['result'];
  $message = $response['message'];
  if ($response['result'] != '00') {
    return FALSE;
  }
  $pasref = isset($response['pasref']) ? $response['pasref'] : '';
  $authcode = isset($response['authcode']) ? $response['authcode'] : '';

  // This digital signature should correspond to the one Realex Payments returns
  // and can therefore be used to verify the message Realex Payments sends back.
  $tmp = "$timestamp.$merchantid.$orderid.$resultcode.$message.$pasref.$authcode";
  $sha1hash = sha1($tmp);
  $tmp = "$sha1hash.$secret";
  $sha1hash = sha1($tmp);

  $success = FALSE;

  // Check to see if hashes match or not
  if ($sha1hash != $response['sha1hash']) {
    watchdog('commerce_realex', 'Hash match failed for order @order_id.', array('@order_id' => $response['order_id']));
    drupal_set_message(t("Hashes don't match - response not authenticated!"), 'error');
    $response['user_message'] = 'Payment error, please try again later';
  }
  else {
    $success = TRUE;
  }

  unset($response['sha1hash']);

  return $success;
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
function commerce_realex_redirect_transaction_save($payment_method, $order, $response, $log = '') {
  $transaction = commerce_payment_transaction_new('commerce_realex_redirect', $order->order_id);
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $commerce_order_total = $order_wrapper->commerce_order_total->value();
  $transaction->amount = $commerce_order_total['amount'];
  $transaction->currency_code = $commerce_order_total['currency_code'];
  $transaction->instance_id = $payment_method['instance_id'];
  $transaction->remote_id = $response['commerce_order_id'];

  // Set the payment status.
  switch ($response['result']) {
    case '00':
      $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
      break;
    default:
      $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
  }

  // Log the response variables.
  $transaction->message = 'Status @resultcode, @message.<br />Order ID: @orderid,<br />Timestamp: @timestamp<br />';
  $transaction->message_variables = array(
    '@resultcode' => isset($response['result']) ? $response['result'] : '',
    '@message' => isset($response['message']) ? $response['message'] : '',
    '@orderid' => isset($response['order_id']) ? $response['order_id'] : '',
    '@timestamp' => isset($response['timestamp']) ? $response['timestamp'] : '',
  );
  if (!empty($response['authcode'])) {
    $transaction->message .= 'Auth code: @authcode<br />';
    $transaction->message_variables['@authcode'] = $response['authcode'];
  }
  if (!empty($response['pasref'])) {
    $transaction->message .= 'Pasref: @pasref<br />';
    $transaction->message_variables['@pasref'] = $response['pasref'];
  }
  if (!empty($response['avsaddressresult'])) {
    $transaction->message .= 'Address check: @address<br />';
    $transaction->message_variables['@address'] = $response['avsaddressresult'];
  }
  if (!empty($response['avspostcoderesult'])) {
    $transaction->message .= 'Postcode check: @postcode<br />';
    $transaction->message_variables['@postcode'] = $response['avspostcoderesult'];
  }
  if (!empty($response['cvnresult'])) {
    $transaction->message .= 'CVN result: @cvn<br />';
    $transaction->message_variables['@cvn'] = $response['cvnresult'];
  }
  if (!empty($response['batchid'])) {
    $transaction->message .= 'Batch Id: @batchid<br />';
    $transaction->message_variables['@batchid'] = $response['batchid'];
  }
  if (!empty($response['tss'])) {
    $transaction->message .= 'Transaction suitability score: @tss<br />';
    $transaction->message_variables['@tss'] = $response['tss'];
  }
  if (!empty($response['log'])) {
    $transaction->message .= '@log<br />';
    $transaction->message_variables['@log'] = $response['log'];
  }
  if (!empty($response['eci'])) {
    $transaction->message .= 'ECI: @eci_scenario (@eci)<br />';
    $transaction->message_variables['@eci'] = $response['eci'];
    $transaction->message_variables['@eci_scenario'] = commerce_realex_redirect_3dsecure_eci_lookup($response['eci']);
  }
  if (!empty($response['xid'])) {
    $transaction->message .= 'XID: @xid<br />';
    $transaction->message_variables['@xid'] = $response['xid'];
  }
  if (!empty($response['caav'])) {
    $transaction->message .= 'CAAV: @caav<br />';
    $transaction->message_variables['@caav'] = $response['caav'];
  }

  commerce_payment_transaction_save($transaction);

  return $transaction;
}

/**
 * Lookup ECI value returned and map it to scenario.
 */
function commerce_realex_redirect_3dsecure_eci_lookup($eci) {
  $scenario = NULL;
  switch ($eci) {
    case 5:
    case 2:
      $scenario = t('Full 3D Secure – cardholder Enrolled');
      break;
    case 6:
    case 1:
      $scenario = t('Merchant 3D Secure – cardholder not enrolled or attempt ACS server was used');
      break;
    case 7:
    case 0:
      $scenario = t('Non 3D Secure transaction');
      break;
  }
  return $scenario;
}

