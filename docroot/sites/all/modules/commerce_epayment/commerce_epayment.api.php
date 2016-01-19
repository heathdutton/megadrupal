<?php

/**
 * @file
 * API and hooks documentation for the Commerce ePayment module.
 */

/**
 * Allows modules to alter the payment data before it is sent to ePayment.
 *
 * @param &$data
 *   The data that is to be sent to ePayment as an associative array.
 * @param $order
 *   The commerce order object being processed.
 * @param $settings
 *   The configuration settings.
 *
 * @return
 *   No return value.
 */
function hook_commerce_epayment_data_alter(&$data, $order, $settings) {
  // Load the customer profile info for this order and add it's fields to the
  // $data array that will be submitted to ePayment.
  $customer_profile = commerce_customer_profile_load($order->data['profiles']['customer_profile_billing']);
  $profile_address = reset(field_get_items('commerce_customer_profile', $customer_profile, 'commerce_customer_address'));

  $data += array(
    // Billing parameters that will be completed by default on the ePayment form.
    'BILL_FNAME' => (!empty($profile_address['first_name'])) ? $profile_address['first_name'] : '',
    'BILL_LNAME' => (!empty($profile_address['last_name'])) ? $profile_address['last_name'] : '',
    'BILL_ADDRESS' => (!empty($profile_address['thoroughfare'])) ? $profile_address['thoroughfare'] : '',
    'BILL_ZIPCODE' => (!empty($profile_address['postal_code'])) ? $profile_address['postal_code'] : '',
    'BILL_CITY' => (!empty($profile_address['locality'])) ? $profile_address['locality'] : '',
    'BILL_STATE' => (!empty($profile_address['administrative_area'])) ? $profile_address['administrative_area'] : '',
    'BILL_COUNTRYCODE' => (!empty($profile_address['country'])) ? $profile_address['country'] : '',
  );
}
