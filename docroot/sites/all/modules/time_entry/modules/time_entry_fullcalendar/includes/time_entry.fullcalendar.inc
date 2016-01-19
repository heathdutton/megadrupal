<?php
/**
 * @file
 * Provides integration between Time Entry and FullCalendar modules.
 */

/**
 * Implements hook_fullcalendar_options_info().
 */
function time_entry_fullcalendar_options_info() {
  return array(
    'time_entry' => array(
      'name' => t('Time Entry'),
    ),
  );
}

/**
 * Implements hook_fullcalendar_options_definition().
 */
function time_entry_fullcalendar_options_definition() {
  $options['time_entry']['contains'] = array(
    'time_entry_create' => array(
      'default' => FALSE,
    ),
  );

  return $options;
}

/**
 * Implements hook_fullcalendar_options_form().
 */
function time_entry_fullcalendar_options_form(&$form, &$form_state, &$view) {
  $form['time_entry']['time_entry_create'] = array(
    '#type' => 'select',
    '#title' => t('Inline creation Time Entry type'),
    '#description' => t('Inline creation allows creation of new Time Entries directly on the calendar widget. You must choose a Time Entry type in order to use this feature.'),
    '#options' => array(
      0 => t('Disallow inline creation'),
    ),
    '#default_value' => $view->options['time_entry']['time_entry_create'],
  );

  $create = &$form['time_entry']['time_entry_create'];
  $time_entry_types = time_entry_get_types();
  foreach ($time_entry_types as $time_entry_type) {
    $create['#options'][$time_entry_type->type] = $time_entry_type->label;
  }
}

/**
 * Implements hook_fullcalendar_options_process().
 */
function time_entry_fullcalendar_options_process(&$variables, &$settings) {
  $path = drupal_get_path('module', 'time_entry_fullcalendar');
  $js = $path . '/js/time_entry.fullcalendar.js';

  $variables['element']['#attached']['js'][$js] = $js;
}
