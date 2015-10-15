<?php

/**
 * @file
 * Callback methods for Realex Payments Remote integration.
 */

/**
 * Determines whether 3D Secure is supported for the card type.
 *
 * @param $card
 *   Card name.
 * @return
 *   True if card supports 3D secure, false otherwise.
 */
function _commerce_realex_is_3dsecure_supported_card($card) {
  switch ($card) {
    case 'mastercard':
    case 'visa':
    case 'switch':
    case 'maestro':
      return TRUE;
    case 'solo':
    case 'amex':
    case 'laser':
    case 'dc':
    case 'delta':
    default:
      return FALSE;
  }
}

/**
 * Retrieve ECI value for 3D Secure transacions.
 */
function _commerce_realex_get_3dsecure_eci_value($card_type, $scenario) {
  $eci = NULL;
  switch ($card_type) {
    case 'VISA':
      if ($scenario == 'not enrolled') {
        $eci = 6;
      }
      elseif ($scenario == 'no liability shift') {
        $eci = 7;
      }
      break;
    case 'MC':
    case 'SWITCH':
      if ($scenario == 'not enrolled') {
        $eci = 1;
      }
      elseif ($scenario == 'no liability shift') {
        $eci = 0;
      }
      break;
  }
  return $eci;
}

/**
 * Update order to checkout page prior to 3D Secure page.
 * Used in 3D Secure failure situations.
 *
 * @param order
 *   The Commerce Order object.
 */
function commerce_realex_remote_3dsecure_previous_page(&$order) {
  $order_status = commerce_order_status_load($order->status);
  if ($order_status['state'] == 'checkout' && $order_status['checkout_page'] == '3d_secure') {
    $payment_page = commerce_checkout_page_load($order_status['checkout_page']);
    $prev_page = $payment_page['prev_page'];
    $order = commerce_order_status_update($order, 'checkout_' . $prev_page, FALSE, NULL, '');
  }
}

/**
 * Do a 3D Secure verify enrolled request.
 */
function commerce_realex_remote_do_3dsecure_verify_enrolled_request($payment_method, $request, &$order) {
  module_load_include('inc', 'commerce_realex', 'includes/commerce_realex_remote');

  // Send the request.
  $type = '3ds-verifyenrolled';
  $url = $payment_method['settings']['3dsecure_remote_url'];
  $require_liability_shift = $payment_method['settings']['3dsecure_require_liability_shift'];
  $xml = _commerce_realex_remote_build_xml_request($request, $type);
  $response = _commerce_realex_send_request($url, $xml);

  // Verify response.
  $success = _commerce_realex_remote_verify_response($type, $request, $response, $order, $payment_method);
  $transaction = commerce_realex_remote_transaction_save($type, $payment_method, $order, $response);

  // Cardholder is not enrolled, but ACS Url provided, so attempt sending PaReq anyway.
  if (!$success && $response['result'] == 110 && $response['enrolled'] == 'N' && !empty($response['url']) && !empty($response['pareq'])) {
    // Verify sha1hash first, which means temporarily setting result code to a
    // successful "00".
    $response['result'] = '00';
    $success = _commerce_realex_remote_verify_response($type, $request, $response, $order, $payment_method);
    // Reset result code to original.
    $response['result'] = '110';
    // If sha1hash verification succeeded, then $success is TRUE and
    // 3ds-verifysig request will proceed.
  }

  // Successful payment.
  if ($success) {
    // Create a $md variable which is a string of useful information that will be mirrored in the 3d process.
    $md = 'payment_method_id=' . $payment_method['method_id'] . '&orderid=' . $response['orderid'] . '&ccnumber=' . $request['ccnumber'] . '&chname=' . $request['chname'] . '&cctype=' . $request['cctype'] . '&issueno=' . $request['issueno'] . '&currency=' . $request['currency'] . '&amount=' . $request['amount'] . '&expiry_date=' . $request['expiry_date'];
    $key = 'realex' . $response['pareq'] . 'commerce';
    $md = base64_encode(mcrypt_encrypt(MCRYPT_RIJNDAEL_256, md5($key), $md, MCRYPT_MODE_CBC, md5(md5($key))));

    // Save the data to process the 3D Secure response.
    $secure_data = array();
    $secure_data['PAReq'] = $response['pareq'];
    $secure_data['ACSURL'] = $response['url']; // URL to use.
    $secure_data['MD'] = $md; // Merchant data.
    // The URL that the ACS should reply to.
    $secure_data['TermUrl'] = url('commerce-realex/3d-secure-callback/' . $order->order_id, array('absolute' => TRUE));
    // Add the 3D Secure info to the order object.
    $order->data['extra_authorisation'] = $secure_data;
  }

  // Payment failure.
  else {
    // Cardholder is not enrolled, and no URL returned, proceed with auth.
    if ($response['result'] == 110 && $response['enrolled'] == 'N' && empty($response['url'])) {
      $request['eci'] = _commerce_realex_get_3dsecure_eci_value($request['cctype'], 'not enrolled');
      $request['log_message'] .= t('3ds-verifyenrolled: Customer is not 3d enrolled, liability is shifted (Result: @result, Enrolled: @enrolled)', array('@result' => $response['result'], '@enrolled' => $response['enrolled']));
      commerce_realex_remote_do_auth_request($payment_method, $request, $order, $response);
    }

    // Other response codes - proceed with auth if no liability shift required.
    elseif (!$require_liability_shift) {
      $response['enrolled'] = isset($response['enrolled']) ? $response['enrolled'] : '';
      $request['eci'] = _commerce_realex_get_3dsecure_eci_value($request['cctype'], 'no liability shift');
      $request['log_message'] .= t('3ds-verifyenrolled: No liability shift (Result: @result, Enrolled: @enrolled)', array('@result' => $response['result'], '@enrolled' => $response['enrolled']));
      commerce_realex_remote_do_auth_request($payment_method, $request, $order, $response);
    }

    // Other response codes - liability shift required, so stop and show error.
    else {
      $order->data['commerce_realex_remote']['transaction'] = $transaction;
      commerce_realex_remote_3dsecure_previous_page($order);

      drupal_set_message(t('We were unable to process your credit card payment as we were unable to ascertain whether the card is 3D Secure; reason provided by bank: %message. If the problem persists, contact us to complete your order.', array('%message' => $response['message'])), 'warning');
      watchdog('commerce_realex', 'Payment failed: %message', array('%message' => $response['message']), WATCHDOG_WARNING);
      if (empty($_SESSION['realex_failures'])) {
        $_SESSION['realex_failures'] = 0;
      }
      $_SESSION['realex_failures']++;

      $redirect = commerce_checkout_order_uri($order);
      drupal_goto($redirect);
    }
  }
}

/**
 * Callback function for 3D secure.
 *
 * @param order
 *    The Commerce Order object.
 */
function commerce_realex_3dsecure_callback($order) {
  module_load_include('inc', 'commerce_realex', 'includes/commerce_realex_remote');

  // Validate response.
  if (!isset($_POST['MD']) || !isset($_POST['PaRes'])) {
    watchdog('commerce_realex', 'Invalid data received in 3D Secure response', array(), WATCHDOG_ERROR);
    return drupal_not_found();
  }

  $md = $_POST['MD'];
  $key = 'realex' . $order->data['extra_authorisation']['PAReq'] . 'commerce';
  $md = rtrim(mcrypt_decrypt(MCRYPT_RIJNDAEL_256, md5($key), base64_decode($md), MCRYPT_MODE_CBC, md5(md5($key))), "\0");

  // The MD POST variable makes the foundation for our $request for 3ds-verifysig.
  $md = explode('&', $md);
  foreach ($md as $m) {
    list($field, $content) = explode('=', $m);
    $request[$field] = $content;
  }

  $method_id = $request['payment_method_id'];
  $payment_method = commerce_payment_method_load($method_id);
  if (empty($order->payment_methods)) {
    $order->payment_methods = array();
    rules_invoke_all('commerce_payment_methods', $order);
  }
  $payment_method['settings'] = $order->payment_methods[$method_id . '|commerce_payment_' . $method_id]['settings'];

  // Update request array.
  $request['pares'] = $_POST['PaRes'];
  $request['timestamp'] = strftime('%Y%m%d%H%M%S');
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
  mt_srand((double)microtime() * 1000000);

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

  // Build the sha1hash.
  $request['sha1hash'] = _commerce_realex_build_request_sha1hash($request, '3ds-verifysig');

  // Rebuild the xml to check the signature.
  $xml = _commerce_realex_remote_build_xml_request($request, '3ds-verifysig');

  // Send the request.
  $url = $payment_method['settings']['3dsecure_remote_url'];
  $response = _commerce_realex_send_request($url, $xml);

  // Add the mpi fields to the request array.
  if (isset($response['threedsecure_eci'])) {
    $request['3dsecure'] = TRUE;
    $request['eci'] = $response['threedsecure_eci'];
    $request['cavv'] = $response['threedsecure_cavv'];
    $request['xid'] = $response['threedsecure_xid'];
  }
  _commerce_realex_process_3dsecure_verifysig_response($response, $request, $order);
}

/**
 * Process 3d signature response.
 *
 * @param response
 *    An array of the response information.
 * @param request
 *    An array with all the transaction information.
 * @param order
 *    Commerce order object.
 */
function _commerce_realex_process_3dsecure_verifysig_response($response, $request, $order) {
  $send_auth = FALSE;

  if (!isset($response['result'])) {
    watchdog('commerce_realex', 'No results code, something has gone wrong with the transaction.', array(), WATCHDOG_ERROR);
    drupal_goto('checkout/' . $order->order_id . '/payment');
  }

  $method_id = $request['payment_method_id'];
  $payment_method = commerce_payment_method_load($method_id);
  if (empty($order->payment_methods)) {
    $order->payment_methods = array();
    rules_invoke_all('commerce_payment_methods', $order);
  }
  $payment_method['settings'] = $order->payment_methods[$method_id . '|commerce_payment_' . $method_id]['settings'];
  $require_liability_shift = $payment_method['settings']['3dsecure_require_liability_shift'];

  $success = _commerce_realex_remote_verify_response('3ds-verifysig', $request, $response, $order, $payment_method);

  // Successful response.
  if ($success) {
    switch ($response['threedsecure_status']) {
      case 'Y':
        // Cardholder successfully authenticated.
        $request['log_message'] .= t('3ds-verifysig: Cardholder successfully authenticated, liability shift has taken place. (3D Secure status: @status)', array('@status' => $response['threedsecure_status']));
        $send_auth = TRUE;
        break;

      case 'A':
        // The cardholder is enrolled and the bank acknowledges the attempted
        // authentication. Typically 3D secure process failed at the bank and so
        // liability has been shifted to allow the transaction to continue.
        $request['log_message'] .= t('3ds-verifysig: 3D authentication attempt acknowledged, liability shift has taken place. (3D Secure status: @status)', array('@status' => $response['threedsecure_status']));
        $send_auth = TRUE;
        break;

      case 'N':
        // The cardholder did not authenticate successfully – if you authorise
        // this transaction you will be liable for any chargebacks.
        $request['log_message'] .= t('3ds-verifysig: Cardholder failed to authenticate, no liability shift. (3D Secure status: @status)', array('@status' => $response['threedsecure_status']));
        if (!$require_liability_shift) {
          $request['eci'] = _commerce_realex_get_3dsecure_eci_value($request['cctype'], 'no liability shift');
          $send_auth = TRUE;
        }
        $response['user_message'] = t('Authentication failed. Please check your password and try again.');
        break;

      case 'U':
        // Cardholder authentication temporarily unavailable – no liability
        // shift available.
        $request['log_message'] .= t('3ds-verifysig: Cardholder authentication temporarily unavailable, no liability shift. (3D Secure status: @status)', array('@status' => $response['threedsecure_status']));
        if (!$require_liability_shift) {
          $request['eci'] = _commerce_realex_get_3dsecure_eci_value($request['cctype'], 'no liability shift');
          $send_auth = TRUE;
        }
        $response['user_message'] = t('Authentication failed, service temporarily unavailable. Please try again later.');
        break;
      default:
        break;
    }
  }

  // Failure response.
  else {
    $request['log_message'] .= t('3ds-verifysig: Cardholder authentication failed (response code @status), no liability shift.', array('@status' => $response['result']));
    $request['eci'] = _commerce_realex_get_3dsecure_eci_value($request['cctype'], 'no liability shift');
  }

  $log = isset($request['log_message']) ? $request['log_message'] : '';
  $transaction = commerce_realex_remote_transaction_save('3ds-verifysig', $payment_method, $order, $response, $log);
  unset($request['log_message']);

  if ($send_auth) {
    commerce_realex_remote_do_auth_request($payment_method, $request, $order);
  }

  else {
    // No authentication to be sent, inform customer and client what has gone wrong.
    $message = !empty($response['user_message']) ? $response['user_message'] : $response['message'];
    watchdog('commerce_realex', 'Payment failed: %message', array('%message' => $message), WATCHDOG_WARNING);
    drupal_set_message(t('3D Secure verification failed. Payment could not be processed: %message.', array('%message' => $message)), 'error');

    // Get rid of the iframe and redirect to payment failed url.
    $order->data['commerce_realex_remote']['transaction'] = $transaction;
    commerce_realex_remote_3dsecure_previous_page($order);
    $redirect = commerce_checkout_order_uri($order);
    commerce_3d_secure_clear_iframe($redirect);
  }
}

/**
 * Generate a "3Dsecure verifysig" request.
 */
function _commerce_realex_remote_build_xml_request_3dsecure_verifysig($request) {
  $xml = "<request type='3ds-verifysig' timestamp='" . $request['timestamp'] . "'>
    <merchantid>" . $request['merchantid'] . "</merchantid>
    <account>" . $request['account'] . "</account>
    <orderid>" . $request['orderid'] . "</orderid>
    <amount currency='" . $request['currency'] . "'>" . $request['amount'] . "</amount>
    <card>
      <number>" . $request['ccnumber'] . "</number>
      <expdate>" . $request['expiry_date'] . "</expdate>
      <type>" . $request['cctype'] . "</type>
      <chname>" . $request['chname'] . "</chname>
    </card>
    <pares>" . $request['pares'] . "</pares>";
  if (!empty($request['comments']) && is_array($request['comments'])) {
    $xml .= "
      <comments>
        <comment id='1'>" . $request['comments'][0] . "</comment>";
    if (count($request['comments']) > 1) {
      $xml .= "<comment id='2'>" . $request['comments'][1] . "</comment>";
    }
    $xml .= "
      </comments>";
  }
  $xml .= "
    <sha1hash>" . $request['sha1hash'] . "</sha1hash>
  </request>";

  return $xml;
}

/**
 * Generate a "3Dsecure verifyenrolled" request.
 */
function _commerce_realex_remote_build_xml_request_3dsecure_verifyenrolled($request) {
  $xml = "<request type='3ds-verifyenrolled' timestamp='" . $request['timestamp'] . "'>
    <merchantid>" . $request['merchantid'] . "</merchantid>
    <account>" . $request['account'] . "</account>
    <orderid>" . $request['orderid'] . "</orderid>
    <amount currency='" . $request['currency'] . "'>" . $request['amount'] . "</amount>
    <card>
      <number>" . $request['ccnumber'] . "</number>
      <expdate>" . $request['expiry_date'] . "</expdate>
      <type>" . $request['cctype'] . "</type>
      <chname>" . $request['chname'] . "</chname>
    </card>";
  if (!empty($request['comments']) && is_array($request['comments'])) {
    $xml .= "
      <comments>
        <comment id='1'>" . $request['comments'][0] . "</comment>";
    if (count($request['comments']) > 1) {
      $xml .= "<comment id='2'>" . $request['comments'][1] . "</comment>";
    }
    $xml .= "
      </comments>";
  }
  $xml .= "
    <sha1hash>" . $request['sha1hash'] . "</sha1hash>
  </request>";

  return $xml;
}

/**
 * Lookup ECI value returned and map it to scenario.
 */
function commerce_realex_remote_3dsecure_eci_lookup($eci) {
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


