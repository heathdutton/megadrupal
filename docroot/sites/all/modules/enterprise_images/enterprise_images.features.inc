<?php
/**
 * @file
 * enterprise_images.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function enterprise_images_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function enterprise_images_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function enterprise_images_node_info() {
  $items = array(
    'enterprise_image' => array(
      'name' => t('Image'),
      'base' => 'node_content',
      'description' => t('A image file for displaying in galleries'),
      'has_title' => '1',
      'title_label' => t('Image name'),
      'help' => '',
    ),
  );
  return $items;
}
