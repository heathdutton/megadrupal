<?php
/**
 * @file
 * slim_server_side.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function slim_server_side_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "services" && $api == "services") {
    return array("version" => "3");
  }
}

/**
 * Implements hook_views_api().
 */
function slim_server_side_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function slim_server_side_node_info() {
  $items = array(
    'things' => array(
      'name' => t('Things'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
