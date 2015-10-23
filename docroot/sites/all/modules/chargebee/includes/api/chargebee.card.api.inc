<?php
/**
 * @file
 * Card ChargeBee API functions.
 */

/**
 * Convert ChargeBee card object to array.
 *
 * @param object $card
 *   ChargeBee card object.
 *
 * @return array
 *   Card array.
 */
function _chargebee_card_to_array($card) {
  $card_array = array();
  if ($card) {
    $attributes = array(
      'customer_id' => 'customerId',
      'status' => 'status',
      'gateway' => 'gateway',
      'first_name' => 'firstName',
      'last_name' => 'lastName',
      'iin' => 'iin',
      'last4' => 'last4',
      'card_type' => 'cardType',
      'expiry_month' => 'expiryMonth',
      'expiry_year' => 'expiryYear',
      'billing_addr1' => 'billingAddr1',
      'billing_addr2' => 'billingAddr2',
      'billing_city' => 'billingCity',
      'billing_state' => 'billingState',
      'billing_country' => 'billingCountry',
      'billing_zip' => 'billingZip',
      'masked_number' => 'maskedNumber',
    );
    $card_array = _chargebee_object_to_array_convert($card, $attributes);
  }
  return $card_array;
}
