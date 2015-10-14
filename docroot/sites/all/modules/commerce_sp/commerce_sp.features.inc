<?php
/**
 * @file
 * commerce_sp.features.inc
 */

/**
 * Implements hook_commerce_product_default_types().
 */
function commerce_sp_commerce_product_default_types() {
  $items = array(
    'commerce_sp_subscription' => array(
      'type' => 'commerce_sp_subscription',
      'name' => 'Subscription',
      'description' => 'A product type for selling subscriptions.',
      'help' => '',
      'module' => 'commerce_product_ui',
    ),
  );
  return $items;
}
