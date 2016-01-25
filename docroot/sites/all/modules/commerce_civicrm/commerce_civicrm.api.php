<?php


/*
 * $civicrm_address = array(
    1 => array(
      'location_type_id'       => 1,
      'is_primary'             => TRUE,
      'city'                   => $address['locality'],
      'state_province'         => $address['administrative_area'],
      'postal_code'            => $address['postal_code'],
      'street_address'         => $address['thoroughfare'],
      'supplemental_address_1' => $address['premise'],
      'country'                => $address['country']
    )
  );
 */

/**
 * Implements hook_commerce_civicrm_params().
 *
 * @param $params
 *   civicrm_location_update params array.
 * @param $order
 *   Commerce order object.
 * @param $cid
 *   CiviCRM contact id.
 */
function hook_commerce_civicrm_params(&$params, $order, $cid) {
  // You can grab the profile of the customer like so.
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $profile = $order_wrapper->commerce_customer_billing->value();
  $profile_wrapper = entity_metadata_wrapper('commerce_customer_profile', $profile);

  // Then alter $params as required to add any custom commerce fields to send to CiviCRM.
  $params['job_title'] = $profile_wrapper->field_job_title->value();
  $params['current_employer'] = $profile_wrapper->field_organisation->value();
  $params['phone'] = array(
    array(
      'is_primary' => TRUE,
      'phone' => $profile_wrapper->field_phone->value(),
      'phone_type_id' => 1,
      'location_type' => 'Home',
    		'sequential' => 0
    )
  );

  // Update the contact, need this for details like employer, job title etc.
  $params['id'] = $params['contact_id'];
  $contact = civicrm_api('contact', 'update', $params);
  // It would be good to make these mappable through a GUI.
}


/**
 * Implements hook_commerce_civicrm_contribution_params().
 *
 * @param $params
 *   civicrm_location_update params array.
 *   $params = array(
 *     'contact_id' => $cid,
 *     'receive_date' => date('Ymd'),
 *     'total_amount' => $order_total,
 *     'financial_type_id' => variable_get('commerce_civicrm_contribution_type', ''),
 *     'payment_instrument_id' => $payment_instrument_id,
 *     'non_deductible_amount' => 00.00,
 *     'fee_amount' => 00.00,
 *     'net_amount' => $order_total,
 *     'trxn_id' => $txn_id,
 *     'invoice_id' => $order->order_id . '_dc',
 *     'source' => $notes,
 *     'contribution_status_id' => _commerce_civicrm_map_contribution_status($order->status),
 *     'note' => $notes,
 *   );
 * 
 * @param $order
 *   Commerce order object.
 * @param $cid
 *   CiviCRM contact id.
 * @param $transaction
 *   Drupal commerce transaction object
 */
function hook_commerce_civicrm_contribution_params(&$params, $order, $cid, $transaction) {
  // You can grab the order wrapper like so. 
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  // And then adjust the params as required.
}

