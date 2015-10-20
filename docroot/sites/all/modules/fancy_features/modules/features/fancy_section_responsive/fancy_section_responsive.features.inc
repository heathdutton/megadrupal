<?php
/**
 * @file
 * fancy_section_responsive.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function fancy_section_responsive_ctools_plugin_api($module = NULL, $api = NULL) {
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
function fancy_section_responsive_eck_bundle_info() {
  $items = array(
    'fancy_section_responsive' => array(
      'machine_name' => 'fancy_section_responsive',
      'entity_type' => 'fancy_section',
      'name' => 'responsive',
      'label' => 'Responsive',
    ),
  );
  return $items;
}
