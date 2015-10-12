<?php
/**
 * @file
 * odv_citybike.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function odv_citybike_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function odv_citybike_node_info() {
  $items = array(
    'odv_citybike' => array(
      'name' => t('ODV Citybike'),
      'base' => 'node_content',
      'description' => t('Vienna city bike stations.'),
      'has_title' => '1',
      'title_label' => t('Station'),
      'help' => '',
    ),
  );
  return $items;
}
