<?php
/**
 * @file
 * debut_in_the_news.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function debut_in_the_news_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function debut_in_the_news_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => "3.0");
  }
}

/**
 * Implements hook_node_info().
 */
function debut_in_the_news_node_info() {
  $items = array(
    'in_the_news' => array(
      'name' => t('In The News'),
      'base' => 'node_content',
      'description' => t('Stories from external news sources.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
