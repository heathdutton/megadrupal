<?php

/**
 * @file
 * Handles XML-related stuff for Commerce Periship module.
 */

/*
 * This builds the XML to submit to Periship for rates.
 */
function commerce_periship_build_rate_request($order) {
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

  // this returns $weight['unit'] and $weight['weight']
  $weight = commerce_physical_order_weight($order, 'lb');


  /* If there is no total volume or weight for the order, there is no reason
   * to send the request to Periship
   */
  if ($weight['weight'] == NULL) {
    return FALSE;
  }

  $recipient_type = variable_get('commerce_periship_shipto_residential', 0) ? 'R' :'C';
  $signature_type = variable_get('commerce_periship_shipto_signature', 'N');
  $declared_value = $order->commerce_order_total['und']['0']['amount']/100;
  $saturday_delivery = variable_get('commerce_periship_shipto_saturday', 0) ? 'Y' : 'N';

  $ship_date = REQUEST_TIME;
  $quickest_day = variable_get('commerce_periship_quickest_day', 0);
  if ($quickest_day != 0 ) {
    $ship_date = $ship_date + ($quickest_day * 24 * 60 * 60);
  }
  $cutoff_time = variable_get('commerce_periship_cutoff_time', 15);
  if (date('H', $ship_date) >= $cutoff_time ) {  // if current time is after 5pm
    $ship_date = $ship_date + (24 * 60 * 60); //increment ship_date by 24 hours
  }
  if (date('N', $ship_date) == 6 ) { //
    $ship_date = $ship_date + (2 * 24 * 60 * 60); //increment ship_date by 48 hours
  }
  elseif (date('N', $ship_date) == 7 ) {
    $ship_date = $ship_date + (24 * 60 * 60); //increment ship_date by 24 hours
  }
  $ship_date = date('Y-n-j', $ship_date);

  $dry_ice = variable_get('commerce_periship_default_dryice', 0) ? 'Y' : 'N';

  /* Ship To - Customer Shipping Address */
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  // Prepare the shipping address for use in the request.
  if (!empty($order_wrapper->commerce_customer_shipping->commerce_customer_address)) {
    $shipping_address = $order_wrapper->commerce_customer_shipping->commerce_customer_address->value();
  }

  /* Decrypt authentication values */
  $api_vars = commerce_periship_decrypt_vars(TRUE);

  /* Build SimpleXML object for request */
  $request = new SimpleXMLElement('<PeriShipRateRequest/>');

  $header_request = $request->addChild('RequestHeader');
  $header_request->addChild('ShipperID', $api_vars['periship_accountid']);
  $header_request->addChild('ShipperPassword', $api_vars['periship_password']);
  $header_request->addChild('ShipperZIPCode', variable_get('commerce_periship_postal_code', ''));

  $recipient_request = $request->addChild('RecipientInfo');
  //$recipient_request->addChild('RecipientName', $shipping_address['name_line']);
  //$recipient_request->addChild('RecipientStreet', $shipping_address['thoroughfare']);
  //$recipient_request->addChild('RecipientCity', $shipping_address['locality']);
  //$recipient_request->addChild('RecipientState', $shipping_address['administrative_area']);
  $recipient_request->addChild('RecipientZip', $shipping_address['postal_code']);

  $package_request = $request->addChild('PackageInfo');
  $package_request->addChild('Weight', $weight['weight']);
  //$package_request->addChild('Service', ''); //No Need for this. Empty returns all values.
  $package_request->addChild('RecipientType', $recipient_type);
  if ( $signature_type != 'N' ) {
    $package_request->addChild('SignatureType', $signature_type);
  }
  if ($declared_value > 100) {
    $package_request->addChild('DeclaredValue', $declared_value);
  }
  $package_request->addChild('SaturdayDelivery', $saturday_delivery);
  $package_request->addChild('ShipDate', $ship_date);
  $package_request->addChild('DryIce', $dry_ice);

  $type_request = $request->addChild('ReturnType');
  $type_request->addChild('FeeDetail', 'S');

  $xml = (string) $request->asXML(); //convert object to XML
  //Periship requires the absence of the <?xml > line in the argument.
  $xml = preg_replace('/^.+\n/', '', $xml);
  //Remove all line endings to be safe.
  $xml = preg_replace("/\n+/", "", $xml);

  return $xml;
}

/**
 * Submits an API request to Periship.
 *
 * @param $xml
 *   An XML string to submit to Periship.
 * @param $message
 *   Optional log message to use for logged API requests.
 */
function commerce_periship_api_request($xml, $message = '') {
  // Log the API request if specified.
  if (in_array('request', variable_get('commerce_periship_log', array()))) {
    if (empty($message)) {
      $message = t('Submitting API request to Periship');
    }
    watchdog('periship', '@message:<pre>@xml</pre>', array('@message' => $message, '@xml' => $xml));
  }

  $url = 'http://www.periship.com/invoicing/controller/PeriShip.php';
  $data = 'shipment=' . $xml;
  $options = array(
    'headers' => array(
      'Content-Type' => 'application/x-www-form-urlencoded'
    ),
    'method' => 'POST',
    'data' => $data,
  );

  $result = drupal_http_request($url, $options);

  // Log any drupal_http_request errors to the watchdog.
  if ($result->code != 200) {
    watchdog('periship', 'drupal_http_request error @error: @message', array('@error' => $result->code, '@message' => $result->error), WATCHDOG_ERROR);
    return FALSE;
  }

  // Extract the result data into an XML response object.
  $response = new SimpleXMLElement($result->data);

  // Log the API request if specified.
  if (in_array('response', variable_get('commerce_periship_log', array()))) {
    watchdog('periship', 'Periship API response received:<pre>@xml</pre>', array('@xml' => $response->asXML()));
  }

  /* Log any Periship errors to the watchdog. */
  if ($response->ResponseHeader->ErrorCount != '0') {
    foreach ($response->Errors->ErrorItem as $error) {
      /* Extract the error information from the response object.
       * Cast variables as strings since dblog module attempts to serialize them.
       */
      $error_code = (string) $error->ErrorCode;
      $error_description = (string) $error->ErrorDescription;

      /* Display error message to screen */
      drupal_set_message(t('An error occurred during communication with the Periship shipping API. Please check the status logs for the specific error message.'), 'warning', FALSE);
      watchdog('periship', 'Periship error @error: @message', array('@error' => $error_code, '@message' => $error_description), WATCHDOG_ERROR);
    }
    return FALSE;
  }

  return $response;
}
