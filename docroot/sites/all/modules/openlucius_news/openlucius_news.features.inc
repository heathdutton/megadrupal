<?php
/**
 * @file
 * openlucius_news.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function openlucius_news_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function openlucius_news_node_info() {
  $items = array(
    'ol_news' => array(
      'name' => t('News'),
      'base' => 'node_content',
      'description' => t('Content type for global news items that do not belong in a group.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
