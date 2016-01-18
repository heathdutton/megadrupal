<?php
/**
 * @file
 * resource_booking_example.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function resource_booking_example_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function resource_booking_example_views_api() {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function resource_booking_example_node_info() {
  $items = array(
    'event' => array(
      'name' => t('Event'),
      'base' => 'node_content',
      'description' => t('Event uses resources'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'resource' => array(
      'name' => t('Resource'),
      'base' => 'node_content',
      'description' => t('Resource that can be booked'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
  );
  return $items;
}
