<?php
/**
 * @file
 * cash_flow_income.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function cash_flow_income_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "boxes" && $api == "box") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function cash_flow_income_node_info() {
  $items = array(
    'income' => array(
      'name' => t('Income'),
      'base' => 'node_content',
      'description' => t('An income entry. These can be used to pay expenses.'),
      'has_title' => '1',
      'title_label' => t('Description'),
      'help' => '',
    ),
  );
  return $items;
}
