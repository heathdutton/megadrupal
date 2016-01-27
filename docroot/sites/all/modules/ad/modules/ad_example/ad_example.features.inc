<?php
/**
 * @file
 * ad_example.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function ad_example_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "ds" && $api == "ds") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function ad_example_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function ad_example_node_info() {
  $items = array(
    'ad_example' => array(
      'name' => t('Ad Example'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
