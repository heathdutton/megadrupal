<?php
/**
 * @file
 * vms_example.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function vms_example_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function vms_example_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => "3.0");
  }
}

/**
 * Implements hook_node_info().
 */
function vms_example_node_info() {
  $items = array(
    'sidebar_message' => array(
      'name' => t('Sidebar message'),
      'base' => 'node_content',
      'description' => t('This is an content type used for examplifying how Views Menu Support and Menu Item Reference Widget can be used together.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
