<?php
/*
 * @file commerce_ezypay.soap.inc
 * Defines the soap classes needed for EzyPay
 * @copyright Copyright(c) 2011 Rowlands Group
 * @license GPL v3 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Lee Rowlands leerowlands at rowlandsgroup dot com
 * 
 */

/**
 * Provides 
*/
function commerce_ezypay_soap_transaction($payment_method, $order, $pane_values, $charge) {
  $settings = $payment_method['settings'];
  $client = new SoapClient('https://secure.ezypay.com.au/ezypay/service/CustomerService.cfc?wsdl', array(
      "trace"      => 1,
      "exceptions" => 0));
  $wrapper = entity_metadata_wrapper('commerce_order', $order);
  $address = $profile = FALSE;
  $billing_pane_id = 'customer_profile_billing';
  if ($field_name = variable_get('commerce_' . $billing_pane_id . '_field', '')) {
    $profile = $wrapper->{$field_name}->value();
  }
  else {
    // Or try the association stored in the order's data array if no field is set.
    if (!empty($order->data['profiles'][$billing_pane_id])) {
      $profile = commerce_customer_profile_load($order->data['profiles'][$billing_pane_id]);
    }
  }
  if ($profile) {
    $profile_wrapper = entity_metadata_wrapper('commerce_customer_profile', $profile);
    $address = $profile_wrapper->commerce_customer_address->value();
  }
  if (!$address) {
    return (object) array(
      'request_not_made' => TRUE,
      'errors' => array(
        (object) array(
          'message' => t('You must provide an address in order to complete checkout with Ezypay')
        )
      )
    );
  }
  
  // Build our new customer object
  $newCustomer = array(
    'principalId' => $settings['commerce_ezypay_principal_id'],
    // We use the parent order id as the reference number as we don't have
    // a user account yet for anonymous users.
    'principalRefNo' => $order->order_id,
    'distaccid' => $settings['commerce_ezypay_principal_dist_acct'],
    'confirmTC' => $pane_values['ezypay']['tnc'],
    'firstName' => $address['first_name'],
    'lastName' => $address['last_name'],
    'dob' => commerce_ezypay_to_xsd_date($pane_values['ezypay']['dob']),
    'address1_3' =>  $address['thoroughfare'] . $address['premise'],
    'suburb' => $address['locality'],
    'postcode' => $address['postal_code'],
    'email' => $order->mail,
    'email_confirm' => $order->mail,
    'payPlanId' => '0',
    'amount' => 0,
    'firstAmount' => '0.00',
    'addDate' => commerce_ezypay_to_xsd_date(),
    'nextDate' => commerce_ezypay_to_xsd_date(),
    'debitTypeId' => 4,
    'debitMul' => "1",
    'selpaymethod' => $pane_values['credit_card']['card_type'] == 0 ? 'paydetldd' : 'paydetlcc',
    'payMethId' => $pane_values['credit_card']['card_type']
  );
  if ($payment_method['method_id'] == 'ezypay_credit') {
    $newCustomer += array(
      'ccName' => $pane_values['credit_card']['owner'],
      'ccNumber' => $pane_values['credit_card']['number'],
      'ccExpiryDate' => commerce_ezypay_to_xsd_date(array('year' => $pane_values['credit_card']['exp_year'],
                                                          'month' => $pane_values['credit_card']['exp_month'],
                                                          'day' => 01), TRUE)
    );
  }
  else {
    // Debit
    $newCustomer += array(
      'accountName' => $pane_values['credit_card']['owner'],
      'accountNumber' => $pane_values['credit_card']['number'],
      'accountBSB' => $pane_values['credit_card']['bsb']
    );
  }
  try {
    $response = $client->addCustomer($newCustomer);
  }
  catch(SoapFault $error) {
    return (object) array(
      'errors' => array(
        (object) array(
          'message' => $e->faultString
        )
      )
    );
  }
  if ($settings['commerce_ezypay_debug']) {
    watchdog('commerce_ezypay', '<pre>' . $client->__getLastRequest() . '</pre>', array(), WATCHDOG_DEBUG);
    watchdog('commerce_ezypay', '<pre>' . $client->__getLastResponse() . '</pre>', array(), WATCHDOG_DEBUG);
  }
  return $response;
}

/**
 * Returns a date in xsd:date format
 *
 * @param $date string/array
 *   the date to be formatted in xsd format
 *   pass NULL for current date
 *   
 * @param $array bool
 *   TRUE if date is an array with year/month/day keys
 *
 * @return string date formatted in xsd:date format
*/
function commerce_ezypay_to_xsd_date($date = NULL, $array = FALSE) {
  if (!$date) {
    return date('c');
  }
  if ($array) {
    $date_obj = new DateTime("{$date['year']}-{$date['month']}-{$date['day']}");
  }
  else {
    $date_obj = new DateTime($date);
  }
  return $date_obj->format('c');
}

/**
 * Provides new customer id for updating card on file
 * @param $card_on_file object
 *   Commerce Card on File
 *
 * @return int/bool
 *   new customer id or FALSE for failure
*/
function commerce_ezypay_new_customer_id($card_on_file, $values) {
  $payment_method = commerce_payment_method_instance_load($card_on_file->instance_id);
  $settings = $payment_method['settings'];
  $client = new SoapClient('https://secure.ezypay.com.au/ezypay/service/CustomerService.cfc?wsdl', array(
      "trace"      => 1,
      "exceptions" => 0));
  $wrapper = entity_metadata_wrapper('commerce_order', $order);
  $address = $profile = FALSE;
  $billing_pane_id = 'customer_profile_billing';
  $profiles = commerce_customer_profile_load_multiple(array(), array('uid' => $card_on_file->uid));
  if (count($profiles)) {
    $profile = array_pop($profiles);
  }
  if ($profile) {
    $profile_wrapper = entity_metadata_wrapper('commerce_customer_profile', $profile);
    $address = $profile_wrapper->commerce_customer_address->value();
  }
  if (!$address) {
    watchdog('commerce_ezypay', 'Could not retrieve profile information for user @name - could not update card on file with EzyPay',
               array('@name' => $account->name), WATCHDOG_CRITICAL);
    drupal_set_message(t('An error occured whilst attempting to update your card of file. We could not retrieve your address details. Please contact the site administrator.'));
    return FALSE;
  }
  
  list($remote_id, $parent_order_id) = explode('||', $card_on_file->remote_id);
  // Build our new customer object
  $newCustomer = array(
    'principalId' => $settings['commerce_ezypay_principal_id'],
    // Here we pass the original order again, so Ezypay store the new details
    'principalRefNo' => $parent_order_id,
    'distaccid' => $settings['commerce_ezypay_principal_dist_acct'],
    'confirmTC' => 1,
    'firstName' => $address['first_name'],
    'lastName' => $address['last_name'],
    'dob' => commerce_ezypay_to_xsd_date($values['ezypay']['dob']),
    'address1_3' =>  $address['thoroughfare'] . $address['premise'],
    'suburb' => $address['locality'],
    'postcode' => $address['postal_code'],
    'email' => $account->mail,
    'email_confirm' => $account->mail,
    'payPlanId' => '0',
    'amount' => 0,
    'firstAmount' => '0.00',
    'addDate' => commerce_ezypay_to_xsd_date(),
    'nextDate' => commerce_ezypay_to_xsd_date(),
    'debitTypeId' => 4,
    'debitMul' => "1",
    'selpaymethod' => $card_on_file->card_type == 0 ? 'paydetldd' : 'paydetlcc',
    'payMethId' => $card_on_file->card_type
  );
  if ($payment_method['method_id'] == 'ezypay_credit') {
    $newCustomer += array(
      'ccName' => $values['credit_card']['owner'],
      'ccNumber' => $values['credit_card']['number'],
      'ccExpiryDate' => commerce_ezypay_to_xsd_date(array('year' => $values['credit_card']['exp_year'],
                                                          'month' => $values['credit_card']['exp_month'],
                                                          'day' => 01), TRUE)
    );
  }
  else {
    // Debit
    $newCustomer += array(
      'accountName' => $values['credit_card']['owner'],
      'accountNumber' => $values['credit_card']['number'],
      'accountBSB' => $values['credit_card']['bsb']
    );
  }
  try {
    $response = $client->addCustomer($newCustomer);
  }
  catch(SoapFault $error) {
    return (object) array(
      'errors' => array(
        (object) array(
          'message' => $e->faultString
        )
      )
    );
  }
  if ($settings['commerce_ezypay_debug']) {
    watchdog('commerce_ezypay', '<pre>' . $client->__getLastRequest() . '</pre>', array(), WATCHDOG_DEBUG);
    watchdog('commerce_ezypay', '<pre>' . $client->__getLastResponse() . '</pre>', array(), WATCHDOG_DEBUG);
  }
  if (!empty($response->errors) && is_array($response->errors)
      && count($response->errors) > 0) {
    // transaction failed
    foreach ($response->errors as $error) {
      drupal_set_message($error->message);
    }
    return FALSE;
  }
  elseif (!empty($response->newCustomerId)) {
    // transaction succeeded
    return $response->newCustomerId;
  }
  else {
    // Something unexpected happened here no errors but no newCustomerId, so we
    // supply some default values.
    if (empty($response->request_not_made)) {
      $response_text = array(t('Failed to receive a response.'));
      // Log the trouble string to the watchdog.
      watchdog('commerce_ezypay', 'Failed to receive a response.', array(), WATCHDOG_ERROR);
    }
    else {
      $response_text = array(t('Failed to make the request.'));
      // Log the trouble string to the watchdog.
      watchdog('commerce_ezypay', 'Failed to make the request.', array(), WATCHDOG_ERROR);
    }
    drupal_set_message(t('An error occured whilst attempting to update your card of file. Please contact the site administrator.'));
    return FALSE;
  }
}