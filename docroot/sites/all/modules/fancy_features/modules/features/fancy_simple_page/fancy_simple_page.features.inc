<?php
/**
 * @file
 * fancy_simple_page.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function fancy_simple_page_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function fancy_simple_page_node_info() {
  $items = array(
    'fancy_simple_page' => array(
      'name' => t('Fancy Simple Page'),
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
