<?php
/**
 * @file
 * cash_flow_payments.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function cash_flow_payments_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function cash_flow_payments_node_info() {
  $items = array(
    'payment' => array(
      'name' => t('Payment'),
      'base' => 'node_content',
      'description' => t('A payment for an expense, drawn from an income line item.'),
      'has_title' => '1',
      'title_label' => t('Description'),
      'help' => '',
    ),
  );
  return $items;
}
