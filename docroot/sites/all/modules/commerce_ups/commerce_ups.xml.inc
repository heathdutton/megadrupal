<?php

/**
 * @file
 * Handles XML-related stuff for Commerce UPS module.
 */

/*
 * This builds the XML to submit to UPS for rates.
 *
 * Here's a decent sample of what the resulting XML should look like: http://sameers.me/2011/01/21/ups-rate-request-sample/
 */
function commerce_ups_build_rate_request($order) {
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

  // Determine the shipping profile reference field name for the order.
  $field_name = commerce_physical_order_shipping_field_name($order);
  $shipping_profile = $order_wrapper->{$field_name}->value();

  // Prepare the shipping address for use in the request.
  if (!empty($order_wrapper->{$field_name}->commerce_customer_address)) {
    $shipping_address = $order_wrapper->{$field_name}->commerce_customer_address->value();
  }
  else {
    $shipping_address = addressfield_default_values();
  }

  $ups_pickupschedule = variable_get('commerce_ups_pick_up_schedule');

  $weight = commerce_physical_order_weight($order, 'lb');  // this returns $weight['unit'] and $weight['weight']
  $volume = commerce_physical_order_volume($order, 'in');  // this returns $volume['unit'] and $weight['volume']

  $default_package_volume = variable_get('commerce_ups_default_package_size_length', '0') * variable_get('commerce_ups_default_package_size_width', '0') * variable_get('commerce_ups_default_package_size_height', '0');

  /* If there is no default package volume, we cannot calculate the number of packages and there is no reason to send to UPS */
  if ($default_package_volume == 0) {
    drupal_set_message(t('There was an error with the UPS configuration.'), 'error', FALSE);
    watchdog('commerce_ups', 'The default measurements for the commerce_ups module is empty or is set to zero. Please set the default package dimensions in the settings page for the commerce_ups module. Without the default measurements this module cannot calculate the number of packages and UPS rates will not be displayed.', array(), WATCHDOG_ALERT);
    return FALSE;
  }

  /* If there is no total volume or weight for the order, there is no reason to send the request to UPS */
  if ($volume['volume'] == NULL || $weight['weight'] == NULL) {
    return FALSE;
  }

  $number_of_packages = ceil($volume['volume'] / $default_package_volume);

  /* Pickup Schedule */
  $schedule_code = variable_get('commerce_ups_pick_up_schedule');

  /* Ship To - Customer Shipping Address */
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  // Prepare the shipping address for use in the request.
  if (!empty($order_wrapper->commerce_customer_shipping->commerce_customer_address)) {
    $shipping_address = $order_wrapper->commerce_customer_shipping->commerce_customer_address->value();
  }

  $api_vars = commerce_ups_decrypt_vars(TRUE);

  $access_request = new SimpleXMLElement('<AccessRequest/>');
  $access_request->addChild('AccessLicenseNumber', $api_vars['ups_accesskey']);
  $access_request->addChild('UserId', $api_vars['ups_userid']);
  $access_request->addChild('Password', $api_vars['ups_password']);

  $rating_request = new SimpleXMLElement('<RatingServiceSelectionRequest/>');

  $request = $rating_request->addChild('Request');
  $transaction_reference = $request->addChild('TransactionReference');
  $transaction_reference->addChild('CustomerContext', 'Bare Bones Rate Request');
  $transaction_reference->addChild('XpciVersion', '1.0001');
  $request->addChild('RequestAction', 'Rate');
  $request->addChild('RequestOption', 'Shop');
  $request->addChild('pickupType')->addChild('code', $schedule_code);

  $shipment = $rating_request->addChild('Shipment');

  $shipper = $shipment->addChild('Shipper');
  $shipper_address = $shipper->addChild('Address');
  $shipper_address->addChild('PostalCode', variable_get('commerce_ups_postal_code', ''));
  $shipper_address->addChild('CountryCode', variable_get('commerce_ups_country_code', ''));
  $shipper->addChild('ShipperNumber', $api_vars['ups_accountid']);

  $shipto = $shipment->addChild('ShipTo');
  $shipto_address = $shipto->addChild('Address');
  $shipto_address->addChild('StateProvinceCode', $shipping_address['administrative_area']);
  $shipto_address->addChild('PostalCode', $shipping_address['postal_code']);
  $shipto_address->addChild('CountryCode', $shipping_address['country']);
  if (variable_get('commerce_ups_shipto_residential', FALSE)) {
    $shipto_address->addChild('ResidentialAddressIndicator', 'true');
  }

  $shipfrom = $shipment->addChild('ShipFrom');
  $shipfrom_address = $shipfrom->addChild('Address');
  $shipfrom_address->addChild('StateProvinceCode', variable_get('commerce_ups_state', ''));
  $shipfrom_address->addChild('PostalCode', variable_get('commerce_ups_postal_code', ''));
  $shipfrom_address->addChild('CountryCode', variable_get('commerce_ups_country_code', ''));
  $shipfrom->addChild('ResidentialAddressIndicator', '');

  $package_number = 1;
  for ($i = 1; $package_number <= $number_of_packages; $i++) {
    $package = $shipment->addChild('Package');
    $package->addChild('PackagingType')->addChild('Code', variable_get('commerce_ups_packaging', '02'));
    $dimensions = $package->addChild('Dimensions');
    $dimensions->addChild('UnitOfMeasurement')->addChild('Code', 'IN');
    $dimensions->addChild('Length', variable_get('commerce_ups_default_package_size_length', '0'));
    $dimensions->addChild('Width', variable_get('commerce_ups_default_package_size_width', '0'));
    $dimensions->addChild('Height', variable_get('commerce_ups_default_package_size_height', '0'));
    $package_weight = $package->addChild('PackageWeight');
    $package_weight->addChild('UnitOfMeasurement')->addChild('Code', 'LBS');
    /* If the weight is less than 0.1, set it to 0.1. I tried to find some "official" documentation
     *   for this on the UPS site, but could not. I did find that other ecommerce platforms are 
     *   using this same logic though, I think it is safe for now. mta
     */
    $package_weight->addChild('Weight', max(array(0.1, $weight['weight'] / $number_of_packages)));
    $package_number++;
  }

  $rating_request .= $rating_request->asXML();
  $xml = $access_request->asXML() . $rating_request;
  return $xml;
}

/**
 * Submits an API request to the Progistics XML Processor.
 *
 * @param $xml
 *   An XML string to submit to the Progistics XML Processor.
 * @param $message
 *   Optional log message to use for logged API requests.
 */
function commerce_ups_api_request($xml, $message = '') {
  // Log the API request if specified.
  if (in_array('request', variable_get('commerce_ups_log', array()))) {
    if (empty($message)) {
      $message = t('Submitting API request to the UPS');
    }
    watchdog('ups', '@message:<pre>@xml</pre>', array('@message' => $message, '@xml' => $xml));
  }

  $ch = curl_init('https://www.ups.com/ups.app/xml/Rate');
  curl_setopt($ch, CURLOPT_HEADER, 0);
  curl_setopt($ch, CURLOPT_POST, 1);
  curl_setopt($ch, CURLOPT_TIMEOUT, 60);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
  curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
  curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_setopt($ch, CURLOPT_POSTFIELDS, $xml);

  $result = curl_exec($ch);

  // Log any errors to the watchdog.
  if ($error = curl_error($ch)) {
    watchdog('ups', 'cURL error: @error', array('@error' => $error), WATCHDOG_ERROR);
    return FALSE;
  }
  curl_close($ch);

  // If we received data back from the server...
  if (!empty($result)) {
    // Extract the result into an XML response object.
    $response = new SimpleXMLElement($result);

    // Log the API request if specified.
    if (in_array('response', variable_get('commerce_ups_log', array()))) {
      watchdog('ups', 'API response received:<pre>@xml</pre>', array('@xml' => $response->asXML()));
    }
    return $response;
  }
  else {
    return FALSE;
  }
}
