<?php
/**
 * @file
 * cash_flow_meta.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function cash_flow_meta_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function cash_flow_meta_views_api() {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function cash_flow_meta_node_info() {
  $items = array(
    'tax_type' => array(
      'name' => t('Tax type'),
      'base' => 'node_content',
      'description' => t('A tax type lets you define taxes to apply to income and saves the hassle of having to enter tax manually every time. These were created to be used with <strong>income</strong> <em>line items</em>.'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
  );
  return $items;
}
