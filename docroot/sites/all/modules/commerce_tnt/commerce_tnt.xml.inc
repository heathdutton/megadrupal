<?php

/**
 * @file
 * Handles XML-related stuff for Commerce TNT module.
 */

/**
 * This builds the XML to submit to TNT for rates.
 */
function commerce_tnt_build_rate_request($order) {

  $shipping_details = field_get_items('commerce_order', $order, 'commerce_customer_shipping');
  $shipping_profile = commerce_customer_profile_load($shipping_details[0]['profile_id']);
  $address_info = field_get_items('commerce_customer_profile', $shipping_profile, 'commerce_customer_address');

  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

  // Loop over each line item on the order.
  foreach ($order_wrapper->commerce_line_items as $delta => $line_item_wrapper) {
    // Get the dimensions value of product line items.
    if (in_array($line_item_wrapper->type->value(), commerce_product_line_item_types())) {
      $line_item_dimensions = commerce_physical_product_line_item_dimensions($line_item_wrapper->value());
      $line_item_weight = commerce_physical_product_line_item_weight($line_item_wrapper->value());

      if (!empty($line_item_weight['weight']) &&
          !physical_field_is_empty($line_item_dimensions, array('type' => 'physical_dimensions'))) {
        // Convert weight to kg (from function commerce_physical_order_weight):
        $converted_weight = physical_weight_convert($line_item_weight, 'kg');
        // Convert the dimensions to "cm" (from function commerce_physical_order_volume):
        $converted_dimensions = physical_dimensions_convert($line_item_dimensions, 'cm');
        $shipping = new stdClass;
        $shipping->length = (string)ceil($converted_dimensions['length']);
        $shipping->width  = (string)ceil($converted_dimensions['width']);
        $shipping->height = (string)ceil($converted_dimensions['height']);
        $shipping->total_weight = number_format($converted_weight['weight'], 2);
        $shipping->number_of_packages='1';
      }
      $shipping_info[] = $shipping;

    }
  }

  if (!empty($shipping_info)) {
    $request = new SimpleXMLElement('<enquiry/>');
    $request->addAttribute('xmlns', 'http://www.tntexpress.com.au');

    $rated = $request->addChild('ratedTransitTimeEnquiry');
    $cutoff = $rated->addChild('cutOffTimeEnquiry');

    $col = $cutoff->addChild('collectionAddress');
    $col->addChild('suburb', variable_get('commerce_tnt_collection_suburb', ''));
    $col->addChild('postCode', variable_get('commerce_tnt_collection_postcode', ''));
    $col->addChild('state', variable_get('commerce_tnt_collection_state', ''));

    $del = $cutoff->addChild('deliveryAddress');
    $del->addChild('suburb', $address_info[0]['locality']);
    $del->addChild('postCode', $address_info[0]['postal_code']);
    $del->addChild('state', $address_info[0]['administrative_area']);

    $cutoff->addChild('shippingDate', _commerce_tnt_first_available_weekday());
    $cutoff->addChild('userCurrentLocalDateTime', date('c'));

    $dangerous = $cutoff->addChild('dangerousGoods');
    $dangerous->addChild('dangerous', 'false');

    $lines = $cutoff->addChild('packageLines');
    $lines->addAttribute('packageType', 'N');

    foreach ($shipping_info as $shipping) {

      $line = $lines->addChild('packageLine');
      $line->addChild('numberOfPackages', $shipping->number_of_packages);

      $dims = $line->addChild('dimensions');
      $dims->addAttribute('unit', 'cm');
      $dims->addChild('length', $shipping->length);
      $dims->addChild('width', $shipping->width);
      $dims->addChild('height', $shipping->height);

      $weight = $line->addChild('weight');
      $weight->addAttribute('unit', 'kg');
      $weight->addChild('weight', $shipping->total_weight);

    }

    $payment = $rated->addChild('termsOfPayment');
    $payment->addChild('senderAccount', variable_get('commerce_tnt_account_number', ''));
    $payment->addChild('payer', 'S');

    return array(
      'username' => variable_get('commerce_tnt_user_id', ''),
      'password' => variable_get('commerce_tnt_password', ''),
      'version' => 2,
      'xmlRequest' => $request->asXML(),
    );
  }
  else {
    return array();
  }
}


/**
 * Submits an API request to TNT.
 *
 * @param string $data
 *   An XML string to submit to TNT.
 */
function commerce_tnt_api_request($data) {

  if (empty($data)) {
    return FALSE;
  }

  $headers = array('Content-Type: application/x-www-form-urlencoded');
  $request = http_build_query($data, '', '&');
  $url = variable_get('commerce_tnt_post_url', '');

  // Using cURL as drupal_http_request returns strange results from the API.
  $ch = curl_init($url);
  curl_setopt($ch, CURLOPT_POST, 1);
  curl_setopt($ch, CURLOPT_TIMEOUT, 30);
  curl_setopt($ch, CURLOPT_POSTFIELDS, $request);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
  curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);

  $result = curl_exec($ch);

  // Log any errors to the watchdog.
  if ($error = curl_error($ch)) {
    watchdog('commerce_tnt', 'cURL error: @error', array('@error' => $error), WATCHDOG_ERROR);
    return FALSE;
  }
  curl_close($ch);

  // If we received data back from the server...
  if (!empty($result)) {
    // Extract the result into an XML response object.
    $response = new SimpleXMLElement($result);
    return $response;
  }
  else {
    return FALSE;
  }
}


/**
 * A helper function to get the first available weekdays date.
 *
 * Note: This does not take public holidays into account.
 */
function _commerce_tnt_first_available_weekday() {
  return date('Y-m-d', strtotime("+1 Weekday"));
}
