<?php
/**
 * @file
 * geocluster_demo.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function geocluster_demo_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function geocluster_demo_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function geocluster_demo_node_info() {
  $items = array(
    'article' => array(
      'name' => t('Article'),
      'base' => 'node_content',
      'description' => t('Use <em>articles</em> for time-sensitive content like news, press releases or blog posts.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
