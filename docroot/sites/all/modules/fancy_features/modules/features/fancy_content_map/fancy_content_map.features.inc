<?php
/**
 * @file
 * fancy_content_map.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function fancy_content_map_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "ds" && $api == "ds") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_eck_bundle_info().
 */
function fancy_content_map_eck_bundle_info() {
  $items = array(
    'fancy_content_map' => array(
      'machine_name' => 'fancy_content_map',
      'entity_type' => 'fancy_content',
      'name' => 'map',
      'label' => 'Map',
    ),
  );
  return $items;
}
