<?php
/**
 * @file
 * ooyala_video_example.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function ooyala_video_example_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function ooyala_video_example_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function ooyala_video_example_node_info() {
  $items = array(
    'video' => array(
      'name' => t('Video'),
      'base' => 'node_content',
      'description' => t('Upload a video to Ooyala.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
