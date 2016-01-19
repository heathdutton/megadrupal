<?php
/**
 * @file
 * odv_bicycle_rack.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function odv_bicycle_rack_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function odv_bicycle_rack_node_info() {
  $items = array(
    'odv_bicycle_rack' => array(
      'name' => t('ODV Bicycle rack'),
      'base' => 'node_content',
      'description' => t('Vienna bicycle racks.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
