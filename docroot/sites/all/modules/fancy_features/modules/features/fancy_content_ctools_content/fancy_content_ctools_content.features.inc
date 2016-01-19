<?php
/**
 * @file
 * fancy_content_ctools_content.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function fancy_content_ctools_content_ctools_plugin_api($module = NULL, $api = NULL) {
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
function fancy_content_ctools_content_eck_bundle_info() {
  $items = array(
    'fancy_content_ctools_content' => array(
      'machine_name' => 'fancy_content_ctools_content',
      'entity_type' => 'fancy_content',
      'name' => 'ctools_content',
      'label' => 'Ctools Content',
    ),
  );
  return $items;
}
