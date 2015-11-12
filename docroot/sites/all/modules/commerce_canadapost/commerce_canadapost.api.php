<?php
/**
 * @file
 * Hooks provided by the Commerce Canada Post module.
 */


/**
 * Allows modules to alter the shipping rate request before it is submitted.
 *
 * @param array &$rate_request
 *   An array of SimpleXML elements containing a shipping rate request (order
 *   data that has been formatted for submission to Canada Post web services).
 *   One element for each package.
 * @param object $order
 *   The order object the shipping rate request is being generated for.
 *
 * @see commerce_canadapost_build_rate_request()
 */
function hook_commerce_canadapost_build_rate_request_alter(&$rate_request, $order) {
  // EXAMPLE: Add 24 hours to the turnaround time if today is Friday.
  if (date('l') == 'Friday') {
    // Get turnaround time (saved in hours).
    $turnaround_time = variable_get('commerce_canadapost_turnaround', 0);

    // Add an additional 24 hours.
    $turnaround_time += 24;

    // Update the rate request.
    foreach ($rate_request as &$rate_request_element) {
      $rate_request_element->{'expected-mailing-date'} = date('Y-m-d', strtotime('+' . $turnaround_time . ' hours'));
    }
  }
}

/**
 * Allow modules to define additional packaging methods.
 *
 * The packaging functions are passed the $order object and must return an array
 * of packages.
 *
 * Package format:
 * $package = array(
 *   'weight' => 99.999,      // REQUIRED: 2.3 digits, in kg
 *   'dimensions' => array(   // OPTIONAL:
 *      'length' => 999.9,    //   REQUIRED: 3.1 digits, in cm
 *      'width' => 999.9,     //   REQUIRED: 3.1 digits, in cm
 *      'height' => 999.9,    //   REQUIRED: 3.1 digits, in cm
 *   ),
 *   'unpackaged' => false,   // OPTIONAL: true/false
 *   'mailing-tube' => false, // OPTIONAL: true/false
 *   'oversized' => false,    // OPTIONAL: true/false
 * );
 *
 * All three dimension attributes are required if 'dimensions' is set. See
 * includes/commerce_canadapost.package.inc for function examples.
 *
 * @param array &$methods
 *   A list of packaging methods (name, callback function, description) keyed by
 *   the method's short name.
 *
 * @see commerce_canadapost_list_packaging_methods()
 */
function hook_commerce_canadapost_list_packaging_methods_alter(&$methods) {
  // Example method.
  $methods['onebox'] = array(
    'name' => t('Everything in One Box'),
    'callback' => 'yourmodule_package_by_onebox',
    'description' => t('This packaging method places all of the products in one
      box regardless of size/weight.'),
  );
}
