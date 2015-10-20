<?php
/**
 * @file
 * enterprise_location.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function enterprise_location_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function enterprise_location_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => "3.0");
  }
}

/**
 * Implements hook_node_info().
 */
function enterprise_location_node_info() {
  $items = array(
    'enterprise_location' => array(
      'name' => t('Location'),
      'base' => 'node_content',
      'description' => t('Stores addresses and geolocation points'),
      'has_title' => '1',
      'title_label' => t('Location name'),
      'help' => '',
    ),
  );
  return $items;
}
