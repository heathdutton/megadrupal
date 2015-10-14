<?php
/**
 * @file
 * contextual_help.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function contextual_help_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function contextual_help_views_api() {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function contextual_help_node_info() {
  $items = array(
    'contextual_help' => array(
      'name' => t('Contextual Help'),
      'base' => 'node_content',
      'description' => t('Help to present in the context of the page in which it\'ll be useful.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
