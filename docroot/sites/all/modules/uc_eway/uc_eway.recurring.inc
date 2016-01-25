<?php

/**
 * @file uc_eway.recurring.inc
 * This file includes the functions that allow uc_recurring to work with
 * eway's token payment system.
 *
 * We use eWAY's token payments api instead of their recurring payments api
 * because it integrates into the uc_recurring framework better.
 *
 * See http://www.eway.com.au/Developer/eway-api/token-payments.aspx
 */

/**
 * Define testing values.
 */
define('UC_EWAY_TOKEN_TEST_CUSTOMER_ID', '9876543211000');
define('UC_EWAY_TOKEN_TEST_CUSTOMER_REF', 'Test 123');

/*******************************************************************************
 * Hooks for uc_recurring
 ******************************************************************************/

/**
 * Implements hook_recurring_info().
 */
function uc_eway_recurring_info() {
  $items = array();

  // eWAY token payments feature.
  $items['eway'] = array(
    'title' => t('Eway Payment Gateway'),
    'module' => 'uc_eway',
    'fee handler' => 'eway',
    'payment method' => 'credit',
    'process callback' => 'uc_eway_token_process',
    'renew callback' => 'uc_eway_token_renew',
    'cancel callback' => 'uc_eway_token_cancel',
    'menu' => array(
      'charge' => UC_RECURRING_MENU_DEFAULT,
      'edit'   => UC_RECURRING_MENU_DEFAULT,
      'update' => array(
        'title' => 'Update Account Details',
        'page arguments' => array('uc_eway_token_update_form'),
        'file' => 'uc_eway.pages.inc',
      ),
      'profile' => array(
        'title' => 'Eway Customer Profile',
        'page arguments' => array('uc_eway_customer_profile_form'),
        'access callback' => 'user_access',
        'access arguments' => array('administer recurring fees'),
        'file' => 'uc_eway.pages.inc',
      ),
      'cancel' => UC_RECURRING_MENU_DEFAULT,

    ),
  );

  return $items;
}

/*******************************************************************************
 * Module functions
 ******************************************************************************/

/**
 * Gets the soap url for Token Payments API.
 */
function uc_eway_token_soap_url() {
  return variable_get('uc_eway_test_mode', UC_EWAY_TEST_MODE_DEFAULT) ? 'https://www.eway.com.au/gateway/ManagedPaymentService/test/managedCreditCardPayment.asmx?WSDL' : 'https://www.eway.com.au/gateway/ManagedPaymentService/managedCreditCardPayment.asmx?WSDL';
}

/**
 * The actual SOAP call using the NuSOAP library.
 */
function _uc_eway_recurring_soap_call($operation, $params) {
  // Display a message to the user about test mode if necessary.
  _uc_eway_display_test_mode_msg();

  $namespace = 'https://www.eway.com.au/gateway/managedpayment';
  $endpoint = uc_eway_token_soap_url();

  $header = '<eWAYHeader xmlns="'. $namespace .'">
    <eWAYCustomerID>' . _uc_eway_get_data('customer_id') . '</eWAYCustomerID>
    <Username>' . _uc_eway_get_data('user_name')  . '</Username>
    <Password>' . _uc_eway_get_data('password')  . '</Password>
    </eWAYHeader>'; // TODO: replace with soapval

  if (@include_once(realpath(_uc_eway_nusoap_path()))) {
    $client = new nusoap_client($endpoint, TRUE);
    $client->response_timeout = 50;
    $result = $client->call($operation, $params, $namespace, '', $header);
  }
  else {
    $result = FALSE;
  }

  foreach ($params as $id => $key) {
    if (in_array($id, array('CCNumber'))) {
       // Don't log the CC number in plan text!
      $params[$id] = '**** **** **** ****';
    }
  }
  // Add a message to watchdog logs if in test mode to inform users of the
  // unusual rounding of the amount values.
  $test_mode_msg = variable_get('uc_eway_test_mode', UC_EWAY_TEST_MODE_DEFAULT) ? t('NOTE: The eWAY module is in test mode. In test mode the amount value will be rounded down to the nearest dollar, which will be zero for values less than 1 dollar.') : '';
  if (_uc_eway_log_to_watchdog()) {
    watchdog('uc_eway', 'SOAP Call (%operation) request: %request, response: %response @test_mode_msg', array('%operation' => $operation, '%request' => var_export($params, TRUE), '%response' => var_export($result, TRUE), '@test_mode_msg' => $test_mode_msg));
  }

  return $result;
}

/**
 * Debug time function.
 */
function _time($type = 'none') {
  return time();
  switch($type) {
    case 'uc_recurring_fee':
      $hour = 7;
      $minute = 0;
      $second = 0;
      $month = 6;
      $day = 27;
      $year = 2009;
      break;
    case 'uc_recurring_renew':
      $hour = 7;
      $minute = 1;
      $second = 0;
      $month = 6;
      $day = 27;
      $year = 2009;
      break;
    default:
      return time();
  }

  $time = mktime($hour, $minute, $second, $month, $day, $year);
  return $time;
}

/**
 * Implements hook_recurring_renew().
 */
function uc_eway_token_renew($order, $fee) {
  global $user;

  $transaction_result = FALSE;

  // If we are doing a product renewal the line items have not been added to
  // the fee_amount, so get the amount including line items.
  $amount = ($fee->module == 'uc_recurring_product') ? $order->order_total : $fee->fee_amount;

  $data = array(
    'managedCustomerID' => $fee->data['ManagedCustomerID'],
    'amount' => round($amount, 2) * 100,  // $amount may not be to 2 decimal places.
    'invoiceReference' => $order->order_id,
    'invoiceDescription' => $fee->fee_title . t('(Original order ID: @oid)', array('@oid' => $fee->order_id)),
  );
  if (variable_get('uc_eway_test_mode', UC_EWAY_TEST_MODE_DEFAULT)) {
    $data['managedCustomerID'] = UC_EWAY_TOKEN_TEST_CUSTOMER_ID;
  }

  $response = _uc_eway_recurring_soap_call('ProcessPayment', $data);

  if ($response['ewayResponse']['ewayTrxnStatus'] != 'True') {
    $result = array(
      'success' => FALSE,
      'message' => t('Credit card payment declined: @message',
                      array('@message' => $response['ewayResponse']['ewayTrxnError'])),
      'uid' => $user->uid,
    );
  }
  // Transaction succeeded.
  else {
    // Build a message for display and comments in the payments table.
    $message = t('!amount recurring fee collected for @customer. (ID: <a href="!url">!fee</a>)',
                  array(
                    '!url' => url('admin/store/orders/recurring/view/fee/' . $fee->rfid),
                    '!fee' => $fee->rfid,
                    '!amount' => uc_currency_format($fee->fee_amount),
                    '@customer' => $fee->data['ManagedCustomerID'],
                  ));
    $result = array(
      'success' => TRUE,
      'comment' => $message,
      'message' => $message,
      'data' => array(
        'module' => 'uc_eway',
        'txn_type' => UC_CREDIT_AUTH_CAPTURE,
        'subscription_id' => $fee->data['ManagedCustomerID'],
      ),
      'uid' => $user->uid,
    );

    // Enter the payment.
    uc_payment_enter($order->order_id, 'credit', $response['ewayResponse']['ewayReturnAmount'] / 100, 0, $result['data'], $message);

    uc_order_comment_save($order->order_id, $user->uid, $message);
    
    $transaction_result = TRUE;
  }

  // Build an admin order comment.
  $comment = t('<b>@status:</b> @message<br />Amount: @amount<br/>Transaction #: @transaction',
    array(
      '@status' => $result['success'] ? t('ACCEPTED') : t('REJECTED'),
      '@message' => $response['ewayResponse']['ewayTrxnError'],
      '@amount' => uc_currency_format($response['ewayResponse']['ewayReturnAmount'] / 100),
      '@transaction' => $response['ewayResponse']['ewayTrxnNumber'],
    ));

  // Save the comment to the order.
  uc_order_comment_save($order->order_id, $user->uid, $comment);

  return $transaction_result;
}

/**
 * Callback for setting up a recurring fee.
 *
 * @param $order
 *   The order object containing billing and shipping information.
 * @param $fee
 *   An array of data describing the recurring fee.
 * @return
 *   TRUE or FALSE indicating the success of the request.
 */
function uc_eway_token_process($order, &$fee) {
  global $user;

  $fee->fee_handler = 'eway';

  $country = uc_get_country_data(array('country_id' => $order->billing_country));

  $data = array(
    'Title' => 'Mr.', // TODO: Don't hard code this when there is a better way.
    'FirstName' => (string) $order->billing_first_name,
    'LastName' => (string) $order->billing_last_name,
    'Address' => (string) $order->billing_street1 . ' ' . $order->billing_street2,
    'Suburb' => (string) $order->billing_city,
    'State' => (string) uc_get_zone_code($order->billing_zone),
    'Company' => (string) $order->billing_company,
    'PostCode' => (string) $order->billing_postal_code,
    'Country' => strtolower($country[0]['country_iso_code_2']),
    'Email' => (string) $order->primary_email,
    'Fax' => '',
    'Phone' => '',
    'Mobile' => '',
    'CustomerRef' => $order->uid,
    'JobDesc' => '',
    'Comments' => '',
    'URL' => '',
    'CCNumber' => $order->payment_details['cc_number'],
    'CCNameOnCard' => $order->billing_first_name . ' ' . $order->billing_last_name,
    'CCExpiryMonth' => sprintf('%02d', $order->payment_details['cc_exp_month']),
    'CCExpiryYear' => substr($order->payment_details['cc_exp_year'], -2),
  );
  if (variable_get('uc_eway_test_mode', UC_EWAY_TEST_MODE_DEFAULT)) {
    $data['CustomerRef'] = UC_EWAY_TOKEN_TEST_CUSTOMER_REF;
  }

  $response = _uc_eway_recurring_soap_call('CreateCustomer', $data);

  if (empty($response['CreateCustomerResult'])) {
    if ($fee->module = 'uc_recurring_product') {
      uc_order_comment_save($order->order_id, $user->uid, t('Eway: Recurring fee setup for recurring product @model failed.', array('@model' => $fee->model)));
    }
    else {
      uc_order_comment_save($order->order_id, $user->uid, t('Eway: Recurring fee setup for recurring order @order_id failed.', array('@order_id' => $fee->order_id)));
    }
    return FALSE;
  }
  $fee->data['ManagedCustomerID'] = $response['CreateCustomerResult'];
  uc_order_comment_save($order->order_id, $user->uid, t('Eway: Recurring fee setup.<br />ManagedCustomerID: @subscription_id', array('@subscription_id' => $response['CreateCustomerResult'])));

  return TRUE;
}

/**
 * Updates a managed cutomer's details in eWAY.
 *
 * @param $order
 *   An ubercart order object
 * @param $fee
 *   Recurring fee object
 * @return
 *   TRUE or FALSE indicating the success of the cancellation.
 *
 * @todo This could have implications in that it updates eWAY's credit card
 *   details but not drupal's/ubercart's credit card details.  So if other
 *   modules want to look up the credit card details on the order for any reason
 *   they will get the wrong credit card details.
 */
function uc_eway_token_update($order, $fee) {
  global $user;

  $country = uc_get_country_data(array('country_id' => $order->billing_country));

  $data = array(
    'managedCustomerID' => $fee->data['ManagedCustomerID'],
    'Title' => 'Mr.', // TODO: Don't hard code this when there is a better way.
    'FirstName' => (string) $order->billing_first_name,
    'LastName' => (string) $order->billing_last_name,
    'Address' => (string) $order->billing_street1 . ' ' . $order->billing_street2,
    'Suburb' => (string) $order->billing_city,
    'State' => (string) uc_get_zone_code($order->billing_zone),
    'Company' => (string) $order->billing_company,
    'PostCode' => (string) $order->billing_postal_code,
    'Country' => strtolower($country[0]['country_iso_code_2']),
    'Email' => (string) $order->primary_email,
    'Fax' => '',
    'Phone' => '',
    'Mobile' => '',
    'CustomerRef' => $order->uid,
    'JobDesc' => '',
    'Comments' => '',
    'URL' => '',
    'CCNumber' => $order->payment_details['cc_number'],
    'CCNameOnCard' => $order->billing_first_name . ' ' . $order->billing_last_name,
    'CCExpiryMonth' => sprintf('%02d', $order->payment_details['cc_exp_month']),
    'CCExpiryYear' => substr($order->payment_details['cc_exp_year'], -2),
  );
  if (variable_get('uc_eway_test_mode', UC_EWAY_TEST_MODE_DEFAULT)) {
    $data['managedCustomerID'] = UC_EWAY_TOKEN_TEST_CUSTOMER_ID;
    $data['CustomerRef'] = UC_EWAY_TOKEN_TEST_CUSTOMER_REF;
  }

  $response = _uc_eway_recurring_soap_call('UpdateCustomer', $data);

  if ($response['UpdateCustomerResult'] != 'true') {
    if (!empty($fee->order_id)) {
      uc_order_comment_save($fee->order_id, $user->uid, t('eway: Subscription @subscription_id updated failed.', array('@subscription_id' => $fee->data['ManagedCustomerID'])));
    }
    return FALSE;
  }

  uc_order_comment_save($fee->order_id, $user->uid, t('eway: Subscription @subscription_id updated.', array('@subscription_id' => $fee->data['ManagedCustomerID'])));

  return TRUE;
}

/**
 * Cancels an recurring subscription.
 *
 * @param $order_id
 *   The ID of the order the recurring fee was attached to.
 * @param $fee
 *   The data array for the recurring fee being canceled.
 * @return
 *   TRUE or FALSE indicating the success of the cancellation.
 */
function uc_eway_token_cancel($fee) {
  return TRUE;
}
