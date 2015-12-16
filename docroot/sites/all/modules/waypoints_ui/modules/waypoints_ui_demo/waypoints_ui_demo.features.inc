<?php
/**
 * @file
 * waypoints_ui_demo.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function waypoints_ui_demo_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function waypoints_ui_demo_node_info() {
  $items = array(
    'waypoints_ui_demo_page' => array(
      'name' => t('Waypoints UI Demo Page'),
      'base' => 'node_content',
      'description' => t('A simple multifield page for demonstrating the waypoints_ui module.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
