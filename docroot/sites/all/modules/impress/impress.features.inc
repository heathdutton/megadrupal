<?php
/**
 * @file
 * impress.features.inc
 */

/**
 * Implements hook_views_api().
 */
function impress_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_ctools_plugin_api().
 */
function impress_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "weight" && $api == "weight") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function impress_node_info() {
  $items = array(
    'impress_slide' => array(
      'name' => t('Impress slide'),
      'base' => 'node_content',
      'description' => t('Create a single slide to be included in an <a href="http://bartaz.github.com/impress.js/">impress</a> slideshow.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
