<?php
/**
 * @file
 * fancy_section_simple.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function fancy_section_simple_ctools_plugin_api($module = NULL, $api = NULL) {
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
function fancy_section_simple_eck_bundle_info() {
  $items = array(
    'fancy_section_simple' => array(
      'machine_name' => 'fancy_section_simple',
      'entity_type' => 'fancy_section',
      'name' => 'simple',
      'label' => 'Simple',
    ),
  );
  return $items;
}
