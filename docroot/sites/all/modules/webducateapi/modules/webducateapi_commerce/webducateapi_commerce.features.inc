<?php
/**
 * @file
 * webducateapi_commerce.features.inc
 */

/**
 * Implements hook_commerce_product_default_types().
 */
function webducateapi_commerce_commerce_product_default_types() {
  $items = array(
    'webducate_course' => array(
      'type' => 'webducate_course',
      'name' => 'Webducate course',
      'description' => '',
      'help' => '',
      'revision' => '1',
    ),
  );
  return $items;
}
