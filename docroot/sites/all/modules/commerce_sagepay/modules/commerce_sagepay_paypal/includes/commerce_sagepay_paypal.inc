<?php
module_load_include('inc', 'commerce_sagepay', 'includes/commerce_sagepay_common');
module_load_include('inc', 'commerce_sagepay', 'includes/commerce_sagepay_utils');

/**
 * Payment method callback: checkout form.
 */
function commerce_sagepay_paypal_submit_form($payment_method, $pane_values,
                                        $checkout_pane, $order) {

  // No form element required here as we do not need to collect further
  // information before sending to PayPal.
}

/**
 * Payment method callback: checkout form submission.
 */
function commerce_sagepay_paypal_submit_form_validate($payment_method,
                                                      $pane_form,
                                                      $pane_values,
                                                      &$order, $charge) {
  // Wrap the order for easier access to data.
  $wrapper = entity_metadata_wrapper('commerce_order', $order);

  $total = commerce_line_items_total($wrapper->commerce_line_items);

  // Add tax if we have sales tax in the order.
  $total['amount'] = $wrapper->commerce_order_total->amount->value();

  // Load customer profile.
  $profile = commerce_customer_profile_load(
    $order->commerce_customer_billing[LANGUAGE_NONE][0]['profile_id']
  );

  // Get user billing address.
  $billing_address = $profile->commerce_customer_address[LANGUAGE_NONE][0];

  // Get user delivery address.
  $delivery_address = NULL;

  if (isset($order->commerce_customer_shipping)) {
    $delivery_profile = commerce_customer_profile_load(
      $order->commerce_customer_shipping[LANGUAGE_NONE][0]['profile_id']
    );
    $delivery_address = $delivery_profile->commerce_customer_address[LANGUAGE_NONE][0];
  }

  $settings = array();
  $data = _commerce_sagepay_encrypted_order($settings, $order, $total,
    $billing_address, $delivery_address, SAGEPAY_PAYPAL, $pane_values);

  // Create a POST to send to SagePay.
  $post = '';
  foreach ($data as $name => $value) {
    $post .= urlencode($name) . '=' . urlencode($value) . '&';
  }

  // Chop off the last &.
  $post = substr($post, 0, -1);

  // Determine the correct url based on the transaction mode.
  switch (variable_get(SAGEPAY_SETTING_TRANSACTION_MODE)) {
    case SAGEPAY_TXN_MODE_LIVE:
      $server_url = SAGEPAY_DIRECT_SERVER_LIVE;
      break;

    case SAGEPAY_TXN_MODE_TEST:
      $server_url = SAGEPAY_DIRECT_SERVER_TEST;
      break;

    case SAGEPAY_TXN_MODE_SHOWPOST:
      $server_url = SAGEPAY_DIRECT_SERVER_SHOWPOST;
      break;

    case SAGEPAY_TXN_MODE_SIMULATION:
      $server_url = SAGEPAY_DIRECT_SERVER_SIMULATION;
      break;

    default:
      $server_url = SAGEPAY_DIRECT_SERVER_SIMULATION;

  }
  drupal_alter('sagepay_url', $server_url, $pane_values);

  // Send post.
  $response = _commerce_sagepay_request_post($server_url, $post);

  // Collect additional data to store in the transaction.
  $response['Amount'] = $wrapper->commerce_order_total->amount->value();

  $result = commerce_sagepay_process_response($payment_method, $order, $response);

  $paypal_redirect = $response['PayPalRedirectURL'];

  if ($paypal_redirect) drupal_goto($paypal_redirect);
  else $result = FALSE;

  return $result;
}

function commerce_sagepay_paypal_submit_form_submit($payment_method,
                                                      $pane_form,
                                                      $pane_values, &$order,
                                                      $charge) {

}


/**
 * Handle a POST callback from Pay Pal to confirm the transaction.
 *
 * Callback will contain the following:
 * VPSProtocol
 * Status (PAYPALOK, MALFORMED, INVALID, ERROR)
 * StatusDetail
 * VPSTxId
 * AddressStatus (NONE, CONFIRMED, UNCONFIRMED)
 * PayerStatus (VERIFIED, UNVERIFIED)
 * DeliverySurname
 * DeliveryFirstNames
 * DeliveryAddress1
 * DeliveryAddress2
 * DeliveryCity
 * DeliveryPostCode
 * DeliveryCountry
 * DeliveryState
 * DeliveryPhone
 * CustomerEMail
 * PayerID
 *
 * Depending on the status code, we then post back to Sage Pay with the
 * following values:
 * VPSProtocol
 * TxType (COMPLETE)
 * VPSTxId
 * Amount
 * Accept (YES, NO)
 *
 *
 * @param $order_id
 * @param $key
 */
function commerce_sagepay_paypal_handle_callback($order_id){
  $order = commerce_order_load($order_id);
//  $payment_key = $order->data['payment_redirect_key'];

//  // Check key against supplied value.
//  if (!$payment_key === $key) {
//    $notification['status'] = 'INVALID';
//    $notification['message'] = t('Payment Redirect key did not match.');
//  }

  if (empty($_POST)) {
    $notification['status'] = 'ERROR';
    $notification['message'] = t('No Payload returned in the notification POST.');
    watchdog('commerce_sagepay', 'VPS Callback URL accessed with no POST data
    submitted.', array(), WATCHDOG_WARNING);
  }

  if (empty($notification)) {

    // Load transactions with a matching order id, remote transaction id
    // and the status "Started".
    // We need to load the original transaction to identify the charge total.
    $conditions = array('order_id' => $order_id,
                        'remote_id' => $_POST['VPSTxId'],
                        'remote_status' => 'PPREDIRECT');

    $transactions = commerce_payment_transaction_load_multiple(array(),
      $conditions);

    // We expect a transaction to be found, so fail if there isn't one.
    if (empty($transactions)) {
      $notification['status'] = 'INVALID';
      $notification['message'] = t('No matching transaction found');
      watchdog('commerce_sagepay', 'No Matching transaction found in
        SagePay Server VPS Callback for order %order_id',
        array('%order_id' => $order_id), WATCHDOG_ERROR);
    }

    // We expect only ONE transaction to be found, so fail if there are more.
    if (count($transactions) > 1) {
      $notification['status'] = 'INVALID';
      $notification['message'] = t('Multiple matching transaction found');
      watchdog('commerce_sagepay', 'Multiple matching transaction found in
      SagePay Server VPS Callback for order %order_id',
        array('%order_id' =>$order_id), WATCHDOG_ERROR);
    }

    // Verify the transaction.
    $transaction_values = array_values($transactions);
    $transaction = $transaction_values[0];

    $payment_method = $transaction->payment_method;
  }

  // Modify the transaction depending on the PayPal Status
  $response_callback = array(
    'VPSProtocol' => '2.23',
    'TxType' => 'COMPLETE',
    'VPSTxId' => $transaction->remote_id,
    'Amount' => commerce_currency_amount_to_decimal($transaction->amount,
      $transaction->currency_code),
  );

  switch($_POST['Status']) {
    case 'PAYPALOK':
      $response_callback['Accept'] = 'YES';
    break;
    case 'MALFORMED':
    case 'INVALID':
    case 'ERROR':
    default:
      $response_callback['Accept'] = 'NO';
      watchdog('commerce_sagepay', 'FAILED Payment callback received from
            PayPal for order %order_id with status code %status',
      array('%order_id' => $order_id,
            '%status' => $_POST['Status']));

    commerce_sagepay_transaction($payment_method, $order, array(),
      $_POST,
      COMMERCE_PAYMENT_STATUS_FAILURE, SAGEPAY_REMOTE_STATUS_INVALID,
      $transaction);
  }

  switch (variable_get(SAGEPAY_SETTING_TRANSACTION_MODE)) {
    case SAGEPAY_TXN_MODE_LIVE:
      $server_url = SAGEPAY_PAYPAL_COMPLETION_LIVE;
      break;

    case SAGEPAY_TXN_MODE_TEST:
      $server_url = SAGEPAY_PAYPAL_COMPLETION_TEST;
      break;
  }

  $query = _commerce_sagepay_array_to_post($response_callback);

  $response = _commerce_sagepay_request_post($server_url, $query);

  // Amount doesn't come back in this callback - but we have it in the
  //  previous one.
  $response['Amount'] = $response_callback['Amount'] * 100;

  // @todo need to get payment method.

  $result = commerce_sagepay_process_response($payment_method, $order, $response);

  $checkout_pages = commerce_checkout_pages();
  $checkout_page = $checkout_pages['payment'];

  if ($checkout_page['next_page']) {
    // Update the order status to reflect the next checkout page.
    $order = commerce_order_status_update(
      $order,
      'checkout_' . $checkout_page['next_page'],
      FALSE,
      TRUE,
      t('Customer continued to the next checkout page via a submit button.')
    );

    // If it happens to be the complete page, process completion now.
    if ($checkout_page['next_page'] == 'complete') {
      commerce_checkout_complete($order);
    }

    // Redirect to the next checkout page.
    drupal_goto('checkout/' . $order->order_id . '/' .
    $checkout_page['next_page']);
  }



}
