<?php

/**
 * @file
 * Handles JSON-related stuff for Commerce NZ Post module.
 */

/**
 * This builds the URL to submit to NZ Post for rates.
 */
function commerce_nzpost_build_rate_request($order) {
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

  // Determine the shipping profile reference field name for the order.
  $field_name = commerce_physical_order_shipping_field_name($order);

  // unused:
  // Prepare the shipping address for use in the request.
  if (!empty($order_wrapper->{$field_name}->commerce_customer_address)) {
    $shipping_address = $order_wrapper->{$field_name}->commerce_customer_address->value();
  }
  else {
    $shipping_address = addressfield_default_values();
  }

  $weight = commerce_physical_order_weight($order, 'kg');
  // This returns $weight['unit'] and $weight['weight'].
  $volume = commerce_physical_order_volume($order, 'mm');
  // This returns $volume['unit'] and $volume['volume'].
  // Convert everything to mm - the interface asks for cms.
  $default_package_volume
    = variable_get('commerce_nzpost_default_package_size_length', '0') * 10 * variable_get('commerce_nzpost_default_package_size_width', '0') * 10 * variable_get('commerce_nzpost_default_package_size_height', '0') * 10;

  // Without default package volume, we cannot calculate the number of packages.
  // There is no reason to send to NZ Post.
  if ($default_package_volume == 0) {
    drupal_set_message(t('There was an error with the NZ Post configuration.'), 'error', FALSE);
    watchdog('commerce_nzpost', 'The default measurements for the commerce_nzpost module is empty or is set to zero. Please set the default package dimensions in the settings page for the commerce_nzpost module. Without the default measurements this module cannot calculate the number of packages and NZ Post rates will not be displayed.', array(), WATCHDOG_ALERT);
    return FALSE;
  }

  // No total volume or weight?
  // No reason to send the request to NZ Post.
  if ($volume['volume'] == NULL || $weight['weight'] == NULL) {
    return FALSE;
  }

  $number_of_packages = ceil($volume['volume'] / $default_package_volume);

  // Prepare the shipping address for use in the request.
  if (!empty($order_wrapper->commerce_customer_shipping->commerce_customer_address)) {
    $shipping_address = $order_wrapper->commerce_customer_shipping->commerce_customer_address->value();
  }

  $attributes = array();

  /*
   * If we know our volume is 10000 can we just assume that height is 105cm?
   * In this case yes, we can, the maths works out.
   * (maximum 'height' on most of these shipping types.
   * only 2-6 day courier will take longer packages).
   * Divide by 105, then sqrt the remainder to find the other two dimensions.
   *
   */

  $max_dimension = 1050;
  $box_volume = $volume['volume'] / $number_of_packages;
  if ($box_volume > $max_dimension) {
    $length = $max_dimension;
    $d      = $box_volume / $max_dimension;
    $width  = $height = ceil(sqrt($d));
  }
  else {
    $length = $width = $height = ceil(pow($box_volume, 1 / 3));
  }

  $attributes['country_code'] = $shipping_address['country'];
  $attributes['length']       = $length;
  $attributes['width']        = $width;
  $attributes['height']       = $height;
  $attributes['weight']       = $weight['weight'] / $number_of_packages;

  return array('packages' => $number_of_packages, 'attributes' => $attributes);
}

/**
 * Submits an API request to NZ Post.
 */
function commerce_nzpost_api_request($attributes, $destination = 'international') {
  $url = 'http://api.nzpost.co.nz/ratefinder/international.json';
  $result = NULL;

  $attributes['api_key'] = variable_get('commerce_nzpost_api_key');

  global $user;
  // Need to obtain value of cart to send to NZ Post as part of query string.
  $order = commerce_cart_order_load($user->uid);
  if ($order) {
    $wrapper                  = entity_metadata_wrapper('commerce_order', $order);
    $line_items               = $wrapper->commerce_line_items;
    $total                    = commerce_line_items_total($line_items);
    $attributes['value']      = $total['amount'] / 100;
    // thickness? Who came up with that NZ Post?
    $attributes['thickness']  = $attributes['width'];
    $attributes['format']     = 'json';

    $query_string = drupal_http_build_query($attributes);
    $url .= '?' . $query_string;

    $options = array(
      'method' => 'GET',
    );

    $result = drupal_http_request($url, $options);
  }

  // Did we receive data back from the server?
  // Returns as JSON, may not be usefull though, return needs checking.
  if (!empty($result)) {
    return $result;
  }
  else {
    return FALSE;
  }
}
