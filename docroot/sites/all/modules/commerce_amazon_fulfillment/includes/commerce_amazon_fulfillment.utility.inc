<?php

/**
 * @file
 */


/**
 * Helper function for placing amazon fulfillment field on bundles.
 *
 * @param string $bundle
 *   Machine name of the bundle.
 */
function commerce_amazon_fulfillment_create_field_instance($bundle) {
  // Add fulfillment boolean field to bundle.
  $instance = array(
    'label' => 'Amazon Fulfillment',
    'widget' => array(
      'weight' => '49',
      'type' => 'options_onoff',
      'active' => 1,
      'settings' => array(
        'display_label' => 0,
      ),
    ),
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'display' => array(
      'default' => array(
        'label' => 'above',
        'type' => 'list_default',
        'settings' => array(),
        'weight' => 1,
      ),
      'line_item' => array(
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array(),
        'weight' => 0,
      ),
      'node_teaser' => array(
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array(),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'commerce_cart_settings' => array(
      'attribute_field' => 0,
      'attribute_widget' => 'select',
      'attribute_widget_title' => 'Amazon Fulfillment',
    ),
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'field_name' => 'field_product_amazon_fulfillment',
    'entity_type' => 'commerce_product',
    'bundle' => $bundle,
  );
  field_create_instance($instance);

  // Add sku field to bundle.
  $instance = array(
    'label' => 'Amazon Fulfillment Sku',
    'widget' => array(
      'weight' => '50',
      'type' => 'text_textfield',
      'active' => 1,
      'settings' => array(
        'size' => '60',
      ),
    ),
    'settings' => array(
      'text_processing' => '0',
      'user_register_form' => FALSE,
    ),
    'display' => array(
      'default' => array(
        'label' => 'above',
        'type' => 'text_default',
        'settings' => array(),
        'weight' => 2,
      ),
      'line_item' => array(
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array(),
        'weight' => 0,
      ),
      'node_teaser' => array(
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array(),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'field_name' => 'field_product_amazon_sku',
    'entity_type' => 'commerce_product',
    'bundle' => $bundle,
  );
  field_create_instance($instance);

}

/**
 * Function for creating amazon fulfillment order.
 *
 * @param int $order_id
 *   ID of the order placed for notating on amazon fulfillment.
 *
 * @param array $items
 *   A keyed array of SKU's and quantity to send to amazon.
 *
 * @param string $address
 *   A shipping address to send the package(s) to.
 *
 * @param string $shipping_speed
 *   The shipping speed to use with amazon fulfillment.
 */
function commerce_amazon_fulfillment_create_order($order_id, $items, $address, $shipping_speed) {
  // Get data for creating an amazon fulfillment request.
  $data = commerce_amazon_fulfillment_create_order_data($order_id, $items, $address, $shipping_speed);
  $request_url = variable_get('amazon_fulfillment_marketplace_endpoint');

  // Send request to amazon for fulfillment.
  $amazon_fulfillment_request = drupal_http_request(
    $request_url,
    array(
      'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
      'method' => 'POST',
      'data' => $data,
      'max_redirect' => 2,
      'timeout' => 30,
    )
  );

  // Error handling.
  if ($amazon_fulfillment_request->code != 200) {
    // Get response data and text.
    $amazon_response_data = simplexml_load_string($amazon_fulfillment_request->data);
    $amazon_response_text = (string) $amazon_response_data->Error->Message;

    // Build text for error message.
    $error_text = t(
      'Error with Amazon fulfillment on order ID# @order_id. Amazon returned: @status_message. Response text: @response_text',
      array(
        '@order_id' => $order_id,
        '@status_message' => $amazon_fulfillment_request->status_message,
        '@response_text' => $amazon_response_text,
      )
    );
    // System log the error.
    watchdog(
      'commerce_amazon_fulfillment',
      $error_text
    );
    // Create event for rules to operate off of.
    rules_invoke_all('commerce_amazon_fulfillment_submission_error', $error_text);
  }
}

/**
 * Function for returning a $_GET style string of data for Amazon fulfillment.
 *
 * @param int $order_id
 *   ID of the order placed for notating on amazon fulfillment.
 *
 * @param array $items
 *   A keyed array of SKU's and quantity to send to amazon.
 *
 * @param string $address
 *   A shipping address to send the package(s) to.
 *
 * @return string
 *   A string formatted for as $_GET parameters.
 */
function commerce_amazon_fulfillment_create_order_data($order_id, $items, $address, $shipping_speed) {

  // Provide basic address values for countries that won't cause amazon to fail.
  if (!isset($address['administrative_area'])) {
    $address['administrative_area'] = '-';
  }
  if (!isset($address['postal_code'])) {
    $address['postal_code'] = '-';
  }

  // Populate basic values of amazon $_GET request.
  $basic_amazon_get_values = array(
    'Version' => '2010-10-01',
    'Action' => 'CreateFulfillmentOrder',
    'AWSAccessKeyId' => variable_get('amazon_fulfillment_keyid'),
    'SellerId' => variable_get('amazon_fulfillment_merchantid'),
    'Timestamp' => gmdate(DATE_ISO8601),
    'SellerFulfillmentOrderId' => $order_id,
    'DisplayableOrderId' => variable_get('amazon_fulfillment_DisplayableOrderIdPrefix', '') . $order_id,
    'DisplayableOrderComment' => variable_get('amazon_fulfillment_displayable_comment'),
    'ShippingSpeedCategory' => $shipping_speed,
    'DestinationAddress.Name' => commerce_amazon_fulfillment_strip_special_characters($address['name_line']),
    'DestinationAddress.Line1' => commerce_amazon_fulfillment_strip_special_characters($address['thoroughfare']),
    'DestinationAddress.CountryCode' => $address['country'],
    'DestinationAddress.StateOrProvinceCode' => $address['administrative_area'],
    'DisplayableOrderDateTime' => gmdate(DATE_ISO8601),
    'FulfillmentPolicy' => variable_get('amazon_fulfillment_FulfillmentPolicy'),
    'SignatureVersion' => '2',
    'SignatureMethod' => 'HmacSHA256',
  );

  // Add Premise/Address second line if filled out.
  if ($address['premise']) {
    $basic_amazon_get_values = array_merge($basic_amazon_get_values, array(
      'DestinationAddress.Line2' => commerce_amazon_fulfillment_strip_special_characters($address['premise']),
    ));
  }

  // Add city if not from Japan.
  if ($address['country'] != 'JP') {
    $basic_amazon_get_values = array_merge($basic_amazon_get_values, array(
      'DestinationAddress.City' => $address['locality'],
    ));
  }

  // Add Postal code if value present.
  if ($address['postal_code']) {
    $basic_amazon_get_values = array_merge($basic_amazon_get_values, array(
      'DestinationAddress.PostalCode' => $address['postal_code'],
    ));
  }

  $item_values = array();
  // Loop through items and append items to $_GET request.
  foreach ($items as $key => $item) {
    $prefix_key = $key + 1;
    $item_prefix = 'Items.member.' . $prefix_key;
    $item_values = array_merge(
      $item_values,
      array(
        $item_prefix . '.SellerSKU' => $item['sku'],
        $item_prefix . '.SellerFulfillmentOrderItemId' => $prefix_key,
        $item_prefix . '.Quantity' => $item['quantity'],
        $item_prefix . '.FulfillmentNetworkSKU' => $item['fulfillment_sku'],
        $item_prefix . '.PerUnitDeclaredValue.Value' => $item['value'],
        $item_prefix . '.PerUnitDeclaredValue.CurrencyCode' => $item['currency'],
      )
    );
  }
  // Combine basic values with item specific values.
  $basic_amazon_get_values = array_merge($basic_amazon_get_values, $item_values);

  // Sort the basic values by key for encoding for amazon signature!
  ksort($basic_amazon_get_values);

  // Initial get string construction, per amazon signature requirements.
  $data_string = drupal_http_build_query($basic_amazon_get_values);
  $request_uri = str_replace(variable_get('amazon_fulfillment_marketplace_url'), '', variable_get('amazon_fulfillment_marketplace_endpoint'));
  $aws_shortened_url = str_replace('https://', '', variable_get('amazon_fulfillment_marketplace_url'));

  // Create string to sign per exact format Amazon AWS is seeking.
  $string_to_sign = "POST\n" .
    $aws_shortened_url . "\n" .
    $request_uri . "\n" . $data_string;

  // Signature. See: http://docs.developer.amazonservices.com/en_US/dev_guide/DG_ClientLibraries.html#DG_OwnClientLibrary__Signatures.
  $signature_values = array(
    'Signature' => base64_encode(hash_hmac(
      "sha256",
      $string_to_sign,
      variable_get('amazon_fulfillment_secretkey'),
      TRUE
    )),
  );
  // Add signature values to data string.
  $data_string = $data_string . '&' . drupal_http_build_query($signature_values);

  return $data_string;
}

/**
 * Helper function for combining assoc arrays as needed for $_GET request.
 *
 * @param string $get_string
 *   The original string for appending. Can be null to start fresh.
 *
 * @param array $values
 *   Associative array of values.
 *
 * @return string
 *   URL ready $_GET paramater string.
 */
function commerce_amazon_fulfillment_build_get_string($get_string, $values) {
  // Loop through all values and combine into URL friendly string.
  foreach ($values as $key => $value) {
    // Check if first time through loop.
    if (!isset($get_string)) {
      $get_string = '?' . $key . '=' . urlencode($value);
    }
    else {
      if ($key != 'Signature') {
        $get_string .= '&' . $key . '=' . urlencode($value);
      }
      else {
        $get_string .= '&' . $key . '=' . $value;
      }

    }
  }
  return $get_string;
}

/**
 * Custom form validation function for amazon fulfillment.
 */
function commerce_amazon_fulfillment_validate($form, &$form_state) {
  // Check if amazon fulfillment checked and that there is a value for sku.
  if ($form_state['values']['field_product_amazon_fulfillment']['und'][0]['value'] == 1 &&
    $form_state['values']['field_product_amazon_sku']['und'][0]['value'] == '') {
    form_set_error('field_product_amazon_sku', t('If Amazon fulfilment has been selected, fulfillment SKU cannot be left empty.'));
  }
}

/**
 * Helper function for removing special characters from strings.
 *
 * @param string $string
 *   String for removing special characters from.
 *
 * @return string
 *   Stripped string.
 */
function commerce_amazon_fulfillment_strip_special_characters($string) {
  $string = preg_replace('/[^a-zA-Z0-9_ %\[\]\.\(\)%&-]/s', ' ', $string);
  $string = str_replace('&', '', $string);
  return $string;
}

