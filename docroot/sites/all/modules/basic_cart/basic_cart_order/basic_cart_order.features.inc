<?php
/**
 * @file
 * basic_cart_order.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function basic_cart_order_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function basic_cart_order_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function basic_cart_order_node_info() {
  $items = array(
    'order' => array(
      'name' => t('Order'),
      'base' => 'node_content',
      'description' => t('Orders placed through the Basic cart module.'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
  );
  return $items;
}
