<?php
/**
 * @file
 * debut_press_release.features.inc
 */

/**
 * Implementation of hook_ctools_plugin_api().
 */
function debut_press_release_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implementation of hook_views_api().
 */
function debut_press_release_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => "3.0");
  }
}

/**
 * Implementation of hook_node_info().
 */
function debut_press_release_node_info() {
  $items = array(
    'press_release' => array(
      'name' => t('Press Release'),
      'base' => 'node_content',
      'description' => t('Use for dated, usually official press releases that are intended for reporters, press agencies, and other publishers.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
