<?php
/**
 * @file
 * enterprise_video.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function enterprise_video_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function enterprise_video_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => "3.0");
  }
}

/**
 * Implements hook_node_info().
 */
function enterprise_video_node_info() {
  $items = array(
    'enterprise_video' => array(
      'name' => t('Video'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Video title'),
      'help' => '',
    ),
  );
  return $items;
}
