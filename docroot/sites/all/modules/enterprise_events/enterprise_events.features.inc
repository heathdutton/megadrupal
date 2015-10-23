<?php
/**
 * @file
 * enterprise_events.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function enterprise_events_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function enterprise_events_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => "3.0");
  }
}

/**
 * Implements hook_node_info().
 */
function enterprise_events_node_info() {
  $items = array(
    'enterprise_event' => array(
      'name' => t('Event'),
      'base' => 'node_content',
      'description' => t('An event that takes place at a specified time.'),
      'has_title' => '1',
      'title_label' => t('Event name'),
      'help' => '',
    ),
  );
  return $items;
}
