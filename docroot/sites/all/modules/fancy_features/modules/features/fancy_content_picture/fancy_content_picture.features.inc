<?php
/**
 * @file
 * fancy_content_picture.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function fancy_content_picture_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "breakpoints" && $api == "default_breakpoint_group") {
    return array("version" => "1");
  }
  if ($module == "breakpoints" && $api == "default_breakpoints") {
    return array("version" => "1");
  }
  if ($module == "ds" && $api == "ds") {
    return array("version" => "1");
  }
  if ($module == "file_entity" && $api == "file_default_displays") {
    return array("version" => "1");
  }
  if ($module == "picture" && $api == "default_picture_mapping") {
    return array("version" => "2");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_eck_bundle_info().
 */
function fancy_content_picture_eck_bundle_info() {
  $items = array(
    'fancy_content_picture' => array(
      'machine_name' => 'fancy_content_picture',
      'entity_type' => 'fancy_content',
      'name' => 'picture',
      'label' => 'Picture',
    ),
  );
  return $items;
}
