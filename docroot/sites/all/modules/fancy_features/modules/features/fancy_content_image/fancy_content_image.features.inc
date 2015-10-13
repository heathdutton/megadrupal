<?php
/**
 * @file
 * fancy_content_image.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function fancy_content_image_ctools_plugin_api($module = NULL, $api = NULL) {
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
function fancy_content_image_eck_bundle_info() {
  $items = array(
    'fancy_content_image' => array(
      'machine_name' => 'fancy_content_image',
      'entity_type' => 'fancy_content',
      'name' => 'image',
      'label' => 'Image',
    ),
  );
  return $items;
}
