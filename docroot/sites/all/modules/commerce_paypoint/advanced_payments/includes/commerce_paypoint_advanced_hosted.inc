<?php

/**
 * @file
 * Functions for Hosted integration method.
 */

/**
 * Implements hook_redirect_form().
 */
function commerce_paypoint_advanced_redirect_form($form, &$form_state, $order, $payment_method) {

  // Return an error if the enabling action's settings haven't been configured.
  if (empty($payment_method['settings'])) {
    drupal_set_message(t('This payment method must be configured by an administrator before it can be used.'), 'error');
    return array();
  }

  $settings = $payment_method['settings'];
  $iframe_enabled = (bool) $settings['integration_type']['use_iframe'];

  // Initialize payment and get the PayPoint response.
  $ppo = commerce_paypoint_advanced_init_payment($order, $settings, $iframe_enabled);
  $response = $ppo->getLastResponse();
  $status = $ppo->getLastStatus();
  $code = $ppo->getLastCode();
  $data = $ppo->getLastData();

  // Create a new payment transaction and setup the amount.
  $transaction = commerce_payment_transaction_new('commerce_paypoint_advanced', $order->order_id);
  $transaction->amount = $ppo->getTransactionMoney('amount');
  $transaction->currency_code = $ppo->getTransactionMoney('currency');
  $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
  // $transaction->instance_id = $auth['instId'];
  $transaction->payload[REQUEST_TIME] = $response;
  // $transaction->payload['VendorTxCode'] = $data['VendorTxCode'];
  // Add the user id to the transaction.
  $transaction->payload['merchantCustomerId'] = $order->uid;

  // Determine the next action depending on the Status Code returned.
  if (empty($code)) {
    //$transaction->payload['vps_status'] = 'TIMEOUT';
    $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
    $transaction->message = 'No valid response from PayPoint';
    commerce_payment_transaction_save($transaction);
    watchdog('commerce_paypoint_advanced', 'No Status code received in PayPoint callback.', array(), WATCHDOG_ERROR);
  }

  $transaction_status = explode(" ", $status);

  switch ($status) {
    case 'SUCCESS':
      $transaction->remote_status = $status;
      $transaction->remote_id = $data->sessionId;
      commerce_payment_transaction_save($transaction);

      if ($iframe_enabled) {

        // Add Javascript to allow iFrame to work.
        drupal_add_js(drupal_get_path('module', 'commerce_paypoint_advanced') . '/js/commerce_paypoint_hosted.js');

        // Build the iFrame for the next step.
        return drupal_get_form('commerce_paypoint_advanced_iframe_form', 'commerce_paypoint/paypoint_waiting_page', $data->redirectUrl);
      }
      else {
        header("Location: " . $data->redirectUrl);
      }

      exit;

    case 'FAILED':
      $reasonCode = $ppo->getLastReasonCode();
      $reasonMsg = $ppo->getMessageFromResponse($reasonCode);
      watchdog('commerce_paypoint_advanced', 'PayPoint transaction failed:\n<br>Response Code: %code\n<br>Message: %msg',
          array('%code' => $reasonCode, '%msg' => $reasonMsg), WATCHDOG_DEBUG);
      $transaction->remote_status = $status;
      $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
      $transaction->message = 'FAIL Response from PayPoint';
      commerce_payment_transaction_save($transaction);
      drupal_goto('checkout/' . $order->order_id . '/payment/return/' . $order->data['payment_redirect_key']);
      break;

    case 'UNKNOWN':
    default:
      $transaction->remote_status = $status;
      $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
      $transaction->message = 'Unknown or invalid response from PayPoint';
      commerce_payment_transaction_save($transaction);
      watchdog('commerce_paypoint_advanced', 'Unrecognised Status response from PayPoint for order %order_id (%response_code)', array('%order_id' => $order->order_id, '%response_code' => $transaction_status[0]), WATCHDOG_ERROR);
      drupal_goto('checkout/' . $order->order_id . '/payment/return/' . $order->data['payment_redirect_key']);
      break;
  }
}

/**
 * Callback to receive and process the notifications (REST_XML) of completed Cash Issue and Cash Payments to.
 *
 * @param string $order_id
 *    The order id.
 * @param string $key
 *    The security key.
 */
function commerce_paypoint_advanced_notify_callback($order_id, $key, $debug_vps = array()) {

  $notification = array();

  $order = commerce_order_load($order_id);

  $payment_key = $order->data['payment_redirect_key'];

  // Check key against supplied value.
  if (!$payment_key === $key) {
    $notification['status'] = 'INVALID';
    $notification['message'] = t('Payment Redirect key did not match.');
  }

  // The function gives us the ability to load in a fake POST dataset for
  // testing purposes. If this is not present, use the $_POST values.
  if (empty($debug_vps)) {
    $vps_data = $_POST;
  }

  if (empty($vps_data)) {
    $notification['status'] = 'ERROR';
    $notification['message'] = t('No Payload returned in the notification POST.');
    watchdog('commerce_paypoint_advanced', 'VPS Callback URL accessed with no POST data
    submitted.', array(), WATCHDOG_WARNING);
  }

  if (empty($notification)) {

    // Load transactions with a matching order id, remote transaction id
    // and the status "Started".
    // We need to load the original transaction to identify the charge total.
    $conditions = array('order_id' => $order_id, 'remote_id' => $vps_data['VPSTxId'], 'remote_status' => 'STARTED');

    $transactions = commerce_payment_transaction_load_multiple(array(), $conditions);

    // We expect a transaction to be found, so fail if there isn't one.
    if (empty($transactions)) {
      $notification['status'] = 'INVALID';
      $notification['message'] = t('No matching transaction found');
      watchdog('commerce_paypoint_advanced', 'No Matching transaction found in PayPoint Hosted VPS Callback for order %order_id', array('%order_id' => $order_id), WATCHDOG_ERROR);
    }

    // We expect only ONE transaction to be found, so fail if there are more.
    if (count($transactions) > 1) {
      $notification['status'] = 'INVALID';
      $notification['message'] = t('Multiple matching transaction found');
      watchdog('commerce_paypoint_advanced', 'Multiple matching transaction found in
      PayPoint Hosted REST_XML Callback for order %order_id',
        array('%order_id' => $order_id), WATCHDOG_ERROR);
    }

    // Verify the transaction.
    $transaction_values = array_values($transactions);
    $transaction = $transaction_values[0];

    /* TODO: Code is based on commerce_sagepay_server.inc
    if ($transaction->payload['VendorTxCode'] == $vps_data["VendorTxCode"]) {

      // Check we have the correct transaction.
      $payment_method = $transaction->payment_method;

      // Get the total and currency from the original transaction.
      $charge = array('amount' => $transaction->amount,
                      'currency_code' => $transaction->currency_code);

      // get the vendor ID
      $vendor_name = variable_get(PAYPOINT_SETTING_VENDOR_NAME);
      if(isset($order->data['paypoint_overrides']['Vendor'])) {
        $vendor_name = $order->data['paypoint_overrides']['Vendor'];
      }

      // Check for tampering.
      $md5_check = array();
      $md5_check[] = _commerce_paypoint_advanced_clean_input(isset($_REQUEST["VPSTxId"]) ? $_REQUEST["VPSTxId"] : '', "Text");
      $md5_check[] = _commerce_paypoint_advanced_clean_input(isset($_REQUEST["VendorTxCode"]) ? $_REQUEST["VendorTxCode"] : '', "VendorTxCode");
      $md5_check[] = _commerce_paypoint_advanced_clean_input(isset($_REQUEST["Status"]) ? $_REQUEST["Status"] : '', "Text");
      $md5_check[] = _commerce_paypoint_advanced_clean_input(isset($_REQUEST["TxAuthNo"]) ? $_REQUEST["TxAuthNo"] : '', "Number");
      $md5_check[] = $vendor_name;
      $md5_check[] = _commerce_paypoint_advanced_clean_input(isset($_REQUEST["AVSCV2"]) ? $_REQUEST["AVSCV2"] : '', "Text");
      $md5_check[] = $transaction->payload['SecurityKey'];
      $md5_check[] = _commerce_paypoint_advanced_clean_input(isset($_REQUEST["AddressResult"]) ? $_REQUEST["AddressResult"] : '', "Text");
      $md5_check[] = _commerce_paypoint_advanced_clean_input(isset($_REQUEST["PostCodeResult"]) ? $_REQUEST["PostCodeResult"] : '', "Text");
      $md5_check[] = _commerce_paypoint_advanced_clean_input(isset($_REQUEST["CV2Result"]) ? $_REQUEST["CV2Result"] : '', "Text");
      $md5_check[] = _commerce_paypoint_advanced_clean_input(isset($_REQUEST["GiftAid"]) ? $_REQUEST["GiftAid"] : '', "Number");
      $md5_check[] = _commerce_paypoint_advanced_clean_input(isset($_REQUEST["3DSecureStatus"]) ? $_REQUEST["3DSecureStatus"] : '', "Text");
      $md5_check[] = _commerce_paypoint_advanced_clean_input(isset($_REQUEST["CAVV"]) ? $_REQUEST["CAVV"] : '', "Text");
      $md5_check[] = _commerce_paypoint_advanced_clean_input(isset($_REQUEST["AddressStatus"]) ? $_REQUEST["AddressStatus"] : '', "Text");
      $md5_check[] = _commerce_paypoint_advanced_clean_input(isset($_REQUEST["PayerStatus"]) ? $_REQUEST['PayerStatus'] : '', "Text");
      $md5_check[] = _commerce_paypoint_advanced_clean_input(isset($_REQUEST["CardType"]) ? $_REQUEST["CardType"] : '', "Text");
      $md5_check[] = _commerce_paypoint_advanced_clean_input(isset($_REQUEST["Last4Digits"]) ? $_REQUEST["Last4Digits"] : '', "Text");
      $md5_check[] = _commerce_paypoint_advanced_clean_input(isset($_REQUEST["DeclineCode"]) ? $_REQUEST["DeclineCode"] : '', "Text");
      $md5_check[] = _commerce_paypoint_advanced_clean_input(isset($_REQUEST["ExpiryDate"]) ? $_REQUEST["ExpiryDate"] : '', "Text");
      $md5_check[] = _commerce_paypoint_advanced_clean_input(isset($_REQUEST["FraudResponse"]) ? $_REQUEST["FraudResponse"] : '', "Text");
      $md5_check[] = _commerce_paypoint_advanced_clean_input(isset($_REQUEST["BankAuthCode"]) ? $_REQUEST["BankAuthCode"] : '', "Text");

      $str_vpssignature = _commerce_paypoint_advanced_clean_input(check_plain($_REQUEST["VPSSignature"]), "Text");
      $str_message = implode('', $md5_check);
      $str_mysignature = strtoupper(md5($str_message));
      if ($str_mysignature == $str_vpssignature) {
        $transaction->payload['vps_status'] = $vps_data['Status'];

        switch ($vps_data['Status']) {
          case 'OK':
            watchdog('commerce_paypoint_advanced', 'OK Payment callback received from
            PayPoint for order %order_id with status code %status',
              array('%order_id' => $order_id,
                    '%status' => $transaction->payload['Status']));
            commerce_paypoint_advanced_transaction($payment_method, $order, $charge,
              $vps_data,
              COMMERCE_PAYMENT_STATUS_SUCCESS, $vps_data['TxType'],
           $transaction);

            $notification['status'] = 'OK';
            $notification['message'] = t('Transaction notification received.');
            break;

          case 'NOTAUTHED':
            watchdog('commerce_paypoint_advanced', 'NOTAUTHED error from PayPoint for order %order_id with message %msg', array('%order_id' => $order_id, '%msg' => $transaction->payload['StatusDetail']), WATCHDOG_ALERT);
            commerce_paypoint_advanced_transaction($payment_method, $order,
              $charge, $vps_data,
              COMMERCE_PAYMENT_STATUS_FAILURE, PAYPOINT_REMOTE_STATUS_FAIL, $transaction);
            $notification['status'] = 'OK';
            $notification['message'] = $transaction->payload['StatusDetail'];
            $notification['cancel'] = TRUE;
            break;

          case 'REJECTED':
            watchdog('commerce_paypoint_advanced', 'REJECTED error from PayPoint for order %order_id with message %msg', array('%order_id' => $order_id, '%msg' => $transaction->payload['StatusDetail']), WATCHDOG_ALERT);
            commerce_paypoint_advanced_transaction($payment_method, $order,
              $charge, $vps_data, COMMERCE_PAYMENT_STATUS_FAILURE,
              PAYPOINT_REMOTE_STATUS_FAIL, $transaction);
            $notification['status'] = 'OK';
            $notification['message'] = $transaction->payload['StatusDetail'];
            $notification['cancel'] = TRUE;
            break;

          case 'ABORT':
            watchdog('commerce_paypoint_advanced', 'ABORT error from PayPoint for order %order_id with message %msg', array('%order_id' => $order_id, '%msg' => $transaction->payload['StatusDetail']), WATCHDOG_ALERT);
            commerce_paypoint_advanced_transaction($payment_method, $order,
              $charge, $vps_data, COMMERCE_PAYMENT_STATUS_FAILURE,
              PAYPOINT_REMOTE_STATUS_FAIL, $transaction);
            $notification['status'] = 'OK';
            $notification['message'] = $transaction->payload['StatusDetail'];
            $notification['cancel'] = TRUE;
            break;

          case 'FAIL':
            watchdog('commerce_paypoint_advanced', 'FAIL error from PayPoint for order %order_id with message %msg', array('%order_id' => $order_id, '%msg' => $transaction->payload['StatusDetail']), WATCHDOG_ERROR);
            commerce_paypoint_advanced_transaction($payment_method, $order,
              $charge, $vps_data, COMMERCE_PAYMENT_STATUS_FAILURE,
              PAYPOINT_REMOTE_STATUS_FAIL, $transaction);
            $notification['status'] = 'OK';
            $notification['message'] = $transaction->payload['StatusDetail'];
            $notification['cancel'] = TRUE;
            break;

          default:
            watchdog('commerce_paypoint_advanced', 'Unknown error from PayPoint for order
            %order_id with message %msg', array('%order_id' => $order_id, '%msg' => $transaction->payload['StatusDetail']), WATCHDOG_ERROR);
            commerce_paypoint_advanced_transaction($payment_method, $order,
              $charge, $vps_data, COMMERCE_PAYMENT_STATUS_FAILURE,
              PAYPOINT_REMOTE_STATUS_FAIL, $transaction);
            $notification['status'] = 'OK';
            $notification['message'] = 'Unexpected Status code received: ' . $vps_data['Status'];
            $notification['cancel'] = TRUE;
        }



      } else {
        $payload = $transaction->payload;
        $payload['vps_status'] = 'TAMPER';
        commerce_paypoint_advanced_transaction($payment_method, $order, array(),
          $payload, COMMERCE_PAYMENT_STATUS_FAILURE, PAYPOINT_REMOTE_STATUS_FAIL, $transaction);
        $notification['status'] = 'INVALID';
        $notification['message'] = t('MD5 did not match - signs of tampering
          .');
      }
    } else {
      $payload = $transaction->payload;
      $payload['vps_status'] = 'TAMPER';
      commerce_paypoint_advanced_transaction($payment_method, $order, array(),
        $payload, COMMERCE_PAYMENT_STATUS_FAILURE, PAYPOINT_REMOTE_STATUS_FAIL, $transaction);
      $notification['status'] = 'INVALID';
      $notification['message'] = t('Vendor TX code did not match - signs of
      tampering
          .');
    }
    */
  }
  // Send response back to PayPoint to indicate we have received and processed.
  $eoln = chr(13) . chr(10);

  if (array_key_exists('cancel', $notification)) {
    $notification['redirect'] = url('checkout/' . $order_id . '/payment/back/' . $payment_key,
      array('absolute' => TRUE));
  }
  else {
    $notification['redirect'] = url('checkout/' . $order_id . '/payment/return/' . $payment_key,
      array('absolute' => TRUE));
  }

  $return_notification = 'Status=' . $notification['status'] . $eoln .
    'RedirectURL=' . $notification['redirect'] . $eoln . 'StatusDetail=' .
    $notification['message'] . $eoln;

  echo $return_notification;

  /*
  dd(__FUNCTION__);
  $vars = compact(array_keys(get_defined_vars()));
  dd(var_export($vars));
  */
  exit;
}
