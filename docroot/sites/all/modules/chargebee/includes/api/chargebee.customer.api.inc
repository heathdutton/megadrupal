<?php
/**
 * @file
 * Customer ChargeBee API functions.
 */

/**
 * Update billing information for customer.
 *
 * @param string $customer_id
 *   Chargebee customer identifier.
 * @param array $params
 *   Params for update.
 *
 * @return array
 *   Customer and card array.
 */
function _chargebee_customer_update_billinginfo($customer_id, $params) {
  $result = ChargeBee_Customer::updateBillingInfo($customer_id, $params);
  $customer = $result->customer();
  $card = $result->card();
  return array(
    'customer' => _chargebee_customer_to_array($customer),
    'card' => _chargebee_card_to_array($card),
  );
}

/**
 * Convert ChargeBee customer object to array.
 *
 * @param object $customer
 *   ChargeBee customer object.
 *
 * @return array
 *   Customer array.
 */
function _chargebee_customer_to_array($customer) {
  $customer_array = array();
  if ($customer) {
    $attributes = array(
      'customer_id' => 'id',
      'first_name' => 'firstName',
      'last_name' => 'lastName',
      'email' => 'email',
      'phone' => 'phone',
      'company' => 'company',
      'vatNumber' => 'vatNumber',
      'auto_collection' => 'autoCollection',
      'created_at' => 'createdAt',
      'card_status' => 'cardStatus',
    );
    $billing_address = isset($customer->billingAddress) ? $customer->billingAddress : NULL;
    $customer_array = _chargebee_object_to_array_convert($customer, $attributes);
    $customer_array['billing_address'] = _chargebee_customer_billing_address_to_array($billing_address);
  }
  return $customer_array;
}

/**
 * Convert ChargeBee customer billing address object to array.
 *
 * @param object $billing_address
 *   Customer billing address object.
 *
 * @return array
 *   Billing address array.
 */
function _chargebee_customer_billing_address_to_array($billing_address) {
  $attributes = array(
    'first_name' => 'firstName',
    'last_name' => 'lastName',
    'email' => 'email',
    'company' => 'company',
    'phone' => 'phone',
    'line1' => 'line1',
    'line2' => 'line2',
    'line3' => 'line3',
    'city' => 'city',
    'state' => 'state',
    'country' => 'country',
    'zip' => 'zip',
  );
  $billing_address_array = _chargebee_object_to_array_convert($billing_address, $attributes);
  return $billing_address_array;
}
