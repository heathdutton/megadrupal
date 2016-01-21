<?php
/**
 * @file
 * tutor_statistics.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function tutor_statistics_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "relation" && $api == "relation_type_default") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function tutor_statistics_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function tutor_statistics_node_info() {
  $items = array(
    'tutor_exercise' => array(
      'name' => t('Exercise'),
      'base' => 'node_content',
      'description' => t('A content type used for script-generated questions.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
