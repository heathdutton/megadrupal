<?php
/**
 * @file
 * minds_eye_vampire.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function minds_eye_vampire_ctools_plugin_api() {
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
function minds_eye_vampire_node_info() {
  $items = array(
    'me_v_character_sheet' => array(
      'name' => t('Mind\'s Eye vampire character sheet'),
      'base' => 'node_content',
      'description' => t('A Mind\'s Eye vampire character sheet.'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
  );
  return $items;
}
