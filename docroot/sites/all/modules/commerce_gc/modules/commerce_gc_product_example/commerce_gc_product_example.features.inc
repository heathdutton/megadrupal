<?php
/**
 * @file
 * commerce_gc_product_example.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function commerce_gc_product_example_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function commerce_gc_product_example_node_info() {
  $items = array(
    'giftcard_product_display' => array(
      'name' => t('Giftcard product display'),
      'base' => 'node_content',
      'description' => t('Sell giftcard products from this type of display.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
