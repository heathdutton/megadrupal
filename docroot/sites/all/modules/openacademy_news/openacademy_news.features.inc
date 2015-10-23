<?php
/**
 * @file
 * openacademy_news.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function openacademy_news_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "ds" && $api == "ds") {
    return array("version" => "1");
  }
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
  if ($module == "panelizer" && $api == "panelizer") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function openacademy_news_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function openacademy_news_node_info() {
  $items = array(
    'openacademy_news' => array(
      'name' => t('News'),
      'base' => 'node_content',
      'description' => t('An important piece of news to post to your site. '),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
