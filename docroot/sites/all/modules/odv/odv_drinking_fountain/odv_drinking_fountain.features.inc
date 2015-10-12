<?php
/**
 * @file
 * odv_drinking_fountain.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function odv_drinking_fountain_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function odv_drinking_fountain_node_info() {
  $items = array(
    'odv_drinking_fountain' => array(
      'name' => t('ODV Drinking fountain'),
      'base' => 'node_content',
      'description' => t('Vienna drinking fountains.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
