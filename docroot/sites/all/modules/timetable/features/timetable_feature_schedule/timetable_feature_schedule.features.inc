<?php
/**
 * @file
 * timetable_feature_schedule.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function timetable_feature_schedule_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function timetable_feature_schedule_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function timetable_feature_schedule_node_info() {
  $items = array(
    'schedule' => array(
      'name' => t('Schedule'),
      'base' => 'node_content',
      'description' => t('A <em>schedule</em> describes a single day of the event, and may have a timetable.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'session' => array(
      'name' => t('Session'),
      'base' => 'node_content',
      'description' => t('A session can be a presentation, a BoF, or a sprint.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
