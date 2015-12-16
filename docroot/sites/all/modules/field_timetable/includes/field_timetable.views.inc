<?php
/**
 * @file
 * Views module hooks for the timetable module.
 */

/**
 * Implements hook_views_plugins().
 */
function field_timetable_views_plugins() {
  $path = drupal_get_path('module', 'field_timetable');
  return array(
    'style' => array(
      'field_timetable' => array(
        'title' => t('Field Timetable'),
        'help' => t('Display field timetable from different nodes as a unique timetable.'),
        'handler' => 'field_timetable_style_plugin',
        'path' => "$path/includes",
        'uses row plugin' => TRUE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'uses grouping' => FALSE,
        'type' => 'normal',
      ),
    ),
  );
}
