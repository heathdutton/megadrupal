<?php
/**
 * @file
 * character_sheet_basic.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function character_sheet_basic_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function character_sheet_basic_node_info() {
  $items = array(
    'character_sheet' => array(
      'name' => t('Character sheet'),
      'base' => 'node_content',
      'description' => t('An example character sheet, demonstrating stats and skills.'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
  );
  return $items;
}
