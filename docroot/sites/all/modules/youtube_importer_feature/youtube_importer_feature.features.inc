<?php
/**
 * @file
 * youtube_importer_feature.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function youtube_importer_feature_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "feeds_tamper" && $api == "feeds_tamper_default") {
    return array("version" => "2");
  }
}

/**
 * Implements hook_node_info().
 */
function youtube_importer_feature_node_info() {
  $items = array(
    'youtube_importer' => array(
      'name' => t('YouTube Importer'),
      'base' => 'node_content',
      'description' => t('This is for placing your feed in. It must be a favorite list, uploads list or playlist. Check here http://code.google.com/apis/youtube/articles/view_youtube_jsonc_responses.html first before going any further!'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'youtube_video' => array(
      'name' => t('YouTube Video'),
      'base' => 'node_content',
      'description' => t('This is an imported YouTube video with all associated info available through the API.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
