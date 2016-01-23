<?php

/**
 * @file
 * Rating functions for the Commerce Canada Post module.
 */


/**
 * Returns a SimpleXML Element containing the rate request for an order.
 *
 * @param object $order
 *   The order to generate a rate request for.
 *
 * @return SimpleXMLElement
 *   A SimpleXML Element containing the rate request or FALSE if a rate request
 *   could not be generated.
 */
function commerce_canadapost_build_rate_request($order) {
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

  // Determine the shipping profile reference field name for the order.
  $field_name = commerce_physical_order_shipping_field_name($order);
  $shipping_profile = $order_wrapper->{$field_name}->value();

  // Prepare the shipping address for use in the request.
  if (!empty($order_wrapper->{$field_name}->commerce_customer_address)) {
    $shipping_address = $order_wrapper->{$field_name}->commerce_customer_address->value();
  }
  else {
    $shipping_address = array(
      'locality' => '',
      'administrative_area' => '',
      'postal_code' => '',
      'country' => '',
    );
  }

  // Calculate order total (products only) and convert it to CAD.
  $order_total = commerce_line_items_total($order_wrapper->commerce_line_items, array('product'));
  $items_price = commerce_currency_convert($order_total['amount'], $order_total['currency_code'], 'CAD');

  // Package the order.
  $packages = commerce_canadapost_build_package_data($order);

  // Build array of rate requests, one for each package.
  $rate_request = array();
  $customer_number = variable_get('commerce_canadapost_customer_number');
  $origin_postal_code = commerce_canadapost_format_postal_code(variable_get('commerce_canadapost_origin'), 'CA');
  $destination_postal_code = commerce_canadapost_format_postal_code($shipping_address['postal_code'], $shipping_address['country']);
  $turnaround_time = variable_get('commerce_canadapost_turnaround');

  foreach ($packages as $package) {
    $rate_request_data = array();

    $rate_request_data['customer-number'] = $customer_number;
    $rate_request_data['origin-postal-code'] = $origin_postal_code;
    $rate_request_data['parcel-characteristics'] = $package;

    // Set shipping destination.
    switch ($shipping_address['country']) {
      case 'CA':
        $rate_request_data['destination']['domestic']['postal-code'] = $destination_postal_code;
        break;

      case 'US':
        $rate_request_data['destination']['united-states']['zip-code'] = $destination_postal_code;
        break;

      default:
        $rate_request_data['destination']['international']['country-code'] = $shipping_address['country'];
    }

    // Calculate expected mailing date.
    if ($turnaround_time) {
      $rate_request_data['expected-mailing-date'] = date('Y-m-d', strtotime('+' . $turnaround_time . ' hours'));
    }

    // Wrap request data into mailing-scenario container and convert to xml.
    $rate_request_element = new SimpleXMLElement('<mailing-scenario xmlns="http://www.canadapost.ca/ws/ship/rate-v2"/>');
    commerce_simplexml_add_children($rate_request_element, $rate_request_data);

    $rate_request[] = $rate_request_element;
  }

  // Allow modules an opportunity to alter the request before it is sent.
  drupal_alter('commerce_canadapost_build_rate_request', $rate_request, $order);

  return $rate_request;
}

/**
 * Builds array of package data for rate request.
 *
 * @param object $order
 *   The order object to build the package data from.
 *
 * @return array
 *   An associative array containing a list of packages and their attributes,
 *   formatted for Canada Post Web Services APIv2.
 */
function commerce_canadapost_build_package_data($order) {
  // Call packaging method.
  module_load_include('inc', 'commerce_canadapost', 'includes/commerce_canadapost.package');

  // Format packages.
  $packages = array();
  foreach (call_user_func(commerce_canadapost_package_callback(), $order) as $package) {
    $package_element = array();

    // Format weight.
    $package_element['weight'] = number_format($package['weight'], 3);

    // Check dimensions.
    if (isset($package['dimensions'])) {
      // Format dimensions (default to 1cm if unset).
      $package_element['dimensions'] = array(
        'length' => isset($package['length']) ? number_format($package['length'], 1) : 1,
        'width' => isset($package['width']) ? number_format($package['width'], 1) : 1,
        'height' => isset($package['height']) ? number_format($package['height'], 1) : 1,
      );
    }

    // Omit flags if false.
    if (isset($package['unpackaged']) && $package['unpackaged']) {
      $package_element['unpackaged'] = 'true';
    }
    if (isset($package['mailing-tube']) && $package['mailing-tube']) {
      $package_element['mailing-tube'] = 'true';
    }
    if (isset($package['oversized']) && $package['oversized']) {
      $package_element['oversized'] = 'true';
    }

    $packages[] = $package_element;
  }

  return $packages;
}

/**
 * Parse a valid XML response into a commerce_shipping rates array.
 *
 * @param SimpleXMLElement $response
 *   A SimpleXML Element containing the response from the API server.
 *
 * @return array
 *   A commerce_shipping rates array indexed by shipping service or an empty
 *   array if there was a problem parsing the data.
 */
function commerce_canadapost_parse_rate_response($response) {
  $rates = array();

  // Build rates array.
  foreach ($response as $quote) {
    $service_code = (string) $quote->{'service-code'};
    $amount = (float) $quote->{'price-details'}->{'due'};
    $currency_code = 'CAD';

    // Add an item to the rates array for the current service.
    $rates[commerce_canadapost_generate_service_machine_name($service_code)] = array(
      'amount' => commerce_currency_decimal_to_amount($amount, $currency_code),
      'currency_code' => $currency_code,
      'data' => array(
        'delivery_date' => (string) $quote->{'service-standard'}->{'expected-delivery-date'},
      ),
    );
  }

  return $rates;
}
