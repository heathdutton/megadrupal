<?php
/**
 * @file
 * ladder.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function ladder_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function ladder_node_info() {
  $items = array(
    'ladder' => array(
      'name' => t('Ladder'),
      'base' => 'node_content',
      'description' => t('A series of Lessons organized into a ladder. Some Ladders can have multiple sections.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
