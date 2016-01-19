<?php
/**
 * @file
 * enterprise_links.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function enterprise_links_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function enterprise_links_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => "3.0");
  }
}

/**
 * Implements hook_node_info().
 */
function enterprise_links_node_info() {
  $items = array(
    'enterprise_link' => array(
      'name' => t('Link'),
      'base' => 'node_content',
      'description' => t('Add links to other web pages'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
