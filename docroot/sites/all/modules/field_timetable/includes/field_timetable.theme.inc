<?php
/**
 * @file
 * Theme and preprocess functions for the jPlayer module.
 */

/**
 * Preprocess function for a timetable.
 */
function template_preprocess_field_timetable_view(&$vars) {

  // Determine a unique timetable ID.
  $vars['timetable_id'] = 'field_timetable_' . $vars['view']->name;

  // Rewrite rows.
  $rows = array();
  foreach ($vars['rows'] as $node) {
    foreach ($node['events'] as $event) {
      $row = array();
      $row['color'] = ($node['color']) ? $node['color'] : '#B56B43';
      $row['allDay'] = FALSE;
      $row['textColor'] = '#000';
      $row['start'] = $event['start_minute'];
      $row['end'] = $event['end_minute'];
      $row['resource'] = $event['day'];
      $row['title'] = ($node['title']) ? $node['title'] : $event['description'];
      $row['title'] = ($row['title']) ? $row['title'] : '';
      $rows[] = $row;
    }
  }

  $vars['rows'] = $rows;

}

/**
 * The main theme function for the jqgrid.
 */
function theme_field_timetable_view($variables) {

  $path = drupal_get_path('module', 'field_timetable');
  // Add all the CSS & JS.
  // Add the core jqgrid library.
  drupal_add_library('field_timetable', 'field_timetable');

  // Options.
  $field_timetable_options = array();

  $field_timetable_options['editable'] = FALSE;
  $field_timetable_options['selectable'] = FALSE;
  $field_timetable_options['selectHelper'] = FALSE;
  $field_timetable_options['minTime'] = $variables['options']['mintime'];
  $field_timetable_options['maxTime'] = $variables['options']['maxtime'];
  $field_timetable_options['slotMinutes'] = $variables['options']['slotminutes'];
  $field_timetable_options['events'] = $variables['rows'];

  $resources = array();

  drupal_add_js(array('field_timetable' => array('#' . $variables['timetable_id'] => $field_timetable_options)), 'setting');
  drupal_add_js($path . '/js/field_timetable.jquery.js');

  unset($field_timetable_options);

  $output = "<div id='" . $variables['timetable_id'] . "'></div>";

  return $output;

}
