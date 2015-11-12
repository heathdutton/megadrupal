<?php
/**
 * @file commerce_shipping_weight_tariff.commerce.inc
 * Contains Commerce hook implementations.
 */

/**
 * Implements hook_commerce_product_type_info().
 */
function commerce_shipping_weight_tariff_commerce_product_type_info() {
  $types = array(
    'shipping_tariff' => array(
      'type' => 'shipping_tariff',
      'name' => t('Shipping tariff'),
      'description' => t('A shipping tariff product.'),
      'help' => t('A shipping tariff defines one price point for a particular shipping service.'),
      'revision' => TRUE,
    ),
  );
  return $types;
}

/**
 * Implements hook_commerce_shipping_method_info().
 *
 * Defines our shipping method, that is, the abstract handling of shipping by
 * weight tariffs.
 */
function commerce_shipping_weight_tariff_commerce_shipping_method_info() {
  $shipping_methods['weight_tariff_shipping'] = array(
    'title' => t('Weight tariff shipping'),
    'description' => t('Defines a shipping rate based on weight times a modifier'),
  );

  return $shipping_methods;
}

function commerce_shipping_weight_tariff_commerce_shipping_service_info() {
  $shipping_services = array();

  $results = db_select('commerce_shipping_weight_tariff_service', 'cswts')
      ->fields('cswts')
      ->execute();

  while($result = $results->fetchAssoc()) {
    $shipping_services[$result['name']] = array(
        'title' => t($result['title']),  //title for the interface
        'description' => t($result['description']),
        'display_title' => t(!empty($result['display_title']) ? $result['display_title'] : $result['title']),
        'shipping_method' => 'weight_tariff_shipping', //comes from the tariff module...
        'price_component' => 'shipping',  //from commerce_shipping
        'callbacks' => array(
        'rate' => 'commerce_shipping_weight_tariff_service_rate',), //rate callback comes from tariff module
    );
  }
  return $shipping_services;
}

/**
 * Implements hook_commerce_shipping_method_collect_rates().
 *
 * Add the tariff product id to the shipping line item data.
 */
function commerce_shipping_weight_tariff_commerce_shipping_method_collect_rates($method, $order) {
  // Get our services.
  $services = commerce_shipping_services('weight_tariff_shipping');

  // Work on each line item that corresponds to one of our shipping services.
  foreach ($order->shipping_rates as $service_name => $shipping_line_item) {
    if (isset($services[$service_name])) {
      // Retrieve the data we (hackishly) put in the price in
      // commerce_shipping_weight_tariff_service_rate().
      $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $shipping_line_item);
      $price_data = $line_item_wrapper->commerce_unit_price->data->value();
      // Entity metadata does not appear to take us any further!?
      $tariff_product_id = $price_data['components'][0]['price']['tariff_product_id'];
      // Add the tariff product id to the line item data.
      $shipping_line_item->data['tariff_product_id'] = $tariff_product_id;
    }
  }
}
