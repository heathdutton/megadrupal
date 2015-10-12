<?php

/**
 * @file
 *
 * Creates agres_availability displays of Views results.
 */
/**
 * Implementation of hook_views_plugins
 */
function agres_availability_views_plugins() {
  $views_path = drupal_get_path('module', 'views');
  $module_path = drupal_get_path('module', 'agres_availability');
  $theme_path = $module_path;
  module_load_include('inc', 'agres_availability', 'theme/theme');

  $data = array(
    'module' => 'agres_availability', // This just tells our themes are elsewhere.
    'style' => array(
      'agres_availability_style' => array(
        'title' => t('agres_availability'),
        'help' => t('Present view results as agreservations Booking agres_availability. (Based on KarenS Calendar Module)'),
        'handler' => 'agres_availability_plugin_style',
        'path' => "$module_path/includes",
        'theme' => 'agres_availability_style',
        'theme file' => 'theme.inc',
        'theme path' => "$module_path/theme",
        'additional themes' => array(
          'agres_availability_mini' => 'style',
          'agres_availability_mini_unittype' => 'style',
          'agres_availability_day' => 'style',
          'agres_availability_week' => 'style',
          'agres_availability_month' => 'style',
          'agres_availability_year' => 'style',
          'agres_availability_day_overlap' => 'style',
          'agres_availability_week_overlap' => 'style',
        ),
        'uses fields' => TRUE,
        'uses grouping' => FALSE,
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'even empty' => TRUE,
      ),
    ),
    'row' => array(
      'agres_availability_node' => array(
        'title' => t('agres_availability Items'),
        'help' => t('Displays each selected node as a agres_availability item.'),
        'handler' => 'agres_availability_plugin_row_node',
        'path' => "$module_path/includes",
        'base' => array('node'), // only works with 'node' as base.
        'uses options' => TRUE,
        'uses fields' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
  return $data;
}
