<?php
/**
 * @file
 * fancy_content_rich_text.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function fancy_content_rich_text_ctools_plugin_api($module = NULL, $api = NULL) {
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
function fancy_content_rich_text_eck_bundle_info() {
  $items = array(
    'fancy_content_rich_text' => array(
      'machine_name' => 'fancy_content_rich_text',
      'entity_type' => 'fancy_content',
      'name' => 'rich_text',
      'label' => 'Rich text',
    ),
  );
  return $items;
}
