<?php
/**
 * @file
 * mirrors_example.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function mirrors_example_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function mirrors_example_node_info() {
  $items = array(
    'no_fields' => array(
      'name' => t('Content no Fields'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'with_fields' => array(
      'name' => t('Content with Fields'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
