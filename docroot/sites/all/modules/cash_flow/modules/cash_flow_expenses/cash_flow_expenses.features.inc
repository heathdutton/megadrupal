<?php
/**
 * @file
 * cash_flow_expenses.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function cash_flow_expenses_ctools_plugin_api() {
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
function cash_flow_expenses_node_info() {
  $items = array(
    'expense' => array(
      'name' => t('Expense'),
      'base' => 'node_content',
      'description' => t('An expense entry. This is the most common thing to enter.'),
      'has_title' => '1',
      'title_label' => t('Description'),
      'help' => '',
    ),
  );
  return $items;
}
