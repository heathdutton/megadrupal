<?php
/**
 * @file
 * opendata_content.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function opendata_content_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function opendata_content_node_info() {
  $items = array(
    'opendata_dataset' => array(
      'name' => t('Dataset'),
      'base' => 'node_content',
      'description' => t('Add an item to your data listing. This content type includes all "common core" required fields described by Project Open Data here: http://project-open-data.github.io/schema/.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => t('Human-readable name of the asset. Should be in plain English and include sufficient detail to facilitate search and discovery.'),
    ),
  );
  return $items;
}
