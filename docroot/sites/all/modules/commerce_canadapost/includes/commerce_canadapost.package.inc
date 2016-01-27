<?php

/**
 * @file
 * Packaging functions for the Commerce Canada Post module.
 */


/**
 * Returns a list of available packaging methods.
 *
 * @return array
 *   A list of packaging methods (name, callback, description) keyed by the
 *   method's short name.
 */
function commerce_canadapost_list_packaging_methods() {
  $methods['weight'] = array(
    'name' => t('By Weight'),
    'callback' => 'commerce_canadapost_package_by_weight',
    'description' => t('Submit just the order weight for rate calculation and
      ignore product dimensions. Orders that are heavier than the maximum amount
      (30kg) will be split into multiple packages, weight distributed evenly.
      Rates are similar regardless of the weight combinations (e.g. the rate
      for two 20kg packages is close to the rate for 16kg + 24kg).'),
  );
  $methods['volumetric'] = array(
    'name' => t('By Volumetric Weight'),
    'callback' => 'commerce_canadapost_package_by_volumetric',
    'description' => t('The volumetric weight of the order is calculated (see
      <a href="@url">Section 3.4 of ABCs of Mailing</a>) and compared with the
      actual order weight. The greater amount is then used to build package data
      using the By Weight method. Use this method if you ship items that are
      light for their size.', array('@url' => 'https://www.canadapost.ca/tools/pg/manual/PGabcmail-e.asp#1378832')),
  );

  // Allow other modules to define additional packaging methods.
  drupal_alter('commerce_canadapost_list_packaging_methods', $methods);

  return $methods;
}

/**
 * Returns the callback function for the selected packaging method.
 *
 * @return string
 *   The callback function name. Defaults to "Package By Weight" method.
 */
function commerce_canadapost_package_callback() {
  $methods = commerce_canadapost_list_packaging_methods();
  $selected = variable_get('commerce_canadapost_packaging_method', 'weight');

  if (isset($methods[$selected]) && function_exists($methods[$selected]['callback'])) {
    return $methods[$selected]['callback'];
  }
  else {
    watchdog('canadapost', 'Packaging function for "!name" method does not exist. Defaulting to "Package By Weight" method.', array('!name' => $methods[$selected]['name']));
    return $methods['weight']['callback'];
  }
}

/**
 * The packaging functions return an array of packages.
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
 * All three dimension attributes are required if 'dimensions' is set.
 */

/**
 * Package By Weight.
 *
 * The data created by this function does not correspond to any real packages.
 *
 * The rates from Canada Post are based on weight (unless a package is
 * considered light for its size), the function creates virtual packages to
 * submit to the rating web service.
 *
 * If a package exceeds the weight limit the order will be divided into multiple
 * virtual packages.
 *
 * Tests have shown that when the order weight is larger than the limit (30kg),
 * the sums of the rates are similar regardless of the weight combinations
 * (e.g. 2 x 20kg vs 16kg + 24kg).
 *
 * @param object $order
 *   The order object.
 *
 * @return array
 *   An associative array of package characteristics.
 */
function commerce_canadapost_package_by_weight($order) {
  // Get the total order weight (in kg) and the weight markup (convert to kg).
  $order_weight = commerce_physical_order_weight($order, 'kg');
  $weight_markup = physical_weight_convert(variable_get('commerce_canadapost_weight_markup', array('weight' => 0, 'unit' => 'kg')), 'kg');

  // Calculate how many packages will be required for the order.
  $num_packages = ceil($order_weight['weight'] / (30 - $weight_markup['weight']));

  // Build package data.
  $package_data = array();

  for ($i = 0; $i < $num_packages; $i++) {
    $package_data[] = array(
      'weight' => round($weight_markup['weight'] + ($order_weight['weight'] / $num_packages), 3),
    );
  }

  return $package_data;
}

/**
 * Package By Volumetric.
 *
 * The data created by this function does not correspond to any real packages.
 *
 * This function calculates the volumetric weight of the order object (see
 * https://www.canadapost.ca/tools/pg/manual/PGabcmail-e.asp#1378832) and
 * compares it with the regular weight.
 *
 * The greater amount is then used to build package data using the same method
 * as commerce_canadapost_package_by_weight().
 *
 * @param object $order
 *   The order object.
 *
 * @return array
 *   An associative array of package characteristics.
 */
function commerce_canadapost_package_by_volumetric($order) {
  // Get the total order weight (in kg) and the weight markup (convert to kg).
  $order_weight = commerce_physical_order_weight($order, 'kg');
  $order_volume = commerce_physical_order_volume($order, 'cm');
  $weight_markup = physical_weight_convert(variable_get('commerce_canadapost_weight_markup', array('weight' => 0, 'unit' => 'kg')), 'kg');

  // Calculate volumetric weight.
  $volumetric_weight = $order_volume['volume'] / 6000;

  // Decide which value to use.
  $shipping_weight = ($volumetric_weight > $order_weight['weight']) ? $volumetric_weight : $order_weight['weight'];

  // Calculate how many packages will be required for the order.
  $num_packages = ceil($shipping_weight / (30 - $weight_markup['weight']));

  // Build package data.
  $package_data = array();

  for ($i = 0; $i < $num_packages; $i++) {
    $package_data[] = array(
      'weight' => round($weight_markup['weight'] + ($shipping_weight / $num_packages), 3),
    );
  }

  return $package_data;
}
