<?php
/**
 * @file
 * iati_feeds.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function iati_feeds_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function iati_feeds_node_info() {
  $items = array(
    'iati_activity_importer' => array(
      'name' => t('IATI activity importer'),
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
