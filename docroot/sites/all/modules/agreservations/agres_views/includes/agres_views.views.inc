<?php

/**
 * @file
 *
 * Creates agres_views displays of Views results.
 */

/**
 * Implements hook_views_plugins().
 */
function agres_views_views_plugins() {
  $views_path = drupal_get_path('module', 'views');
  $module_path = drupal_get_path('module', 'agres_views');
  $theme_path = $module_path;
  module_load_include('inc', 'agres_views', 'theme/theme');

  $data = array(
    'module' => 'agres_views', // This just tells our themes are elsewhere.
    'style' => array(
      'agres_views_style' => array(
        'title' => t('agres_views'),
        'help' => t('Present view results as agreservations Booking agres_views. (Based on KarenS Calendar Module)'),
        'handler' => 'agres_views_plugin_style',
        'path' => "$module_path/includes",
        'theme' => 'agres_views_style',
        'theme file' => 'theme.inc',
        'theme path' => "$module_path/theme",
        'additional themes' => array(
          'agres_views_mini' => 'style',
          'agres_views_day' => 'style',
          'agres_views_week' => 'style',
          'agres_views_month' => 'style',
          'agres_views_year' => 'style',
          'agres_views_day_overlap' => 'style',
          'agres_views_week_overlap' => 'style',
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
      'agres_views_node' => array(
        'title' => t('agres_views Items'),
        'help' => t('Displays each selected node as a agres_views item.'),
        'handler' => 'agres_views_plugin_row_node',
        'path' => "$module_path/includes",
        'base' => array('node'), // only works with 'node' as base.
        'uses options' => TRUE,
        'uses fields' => TRUE,
        'type' => 'normal',
      ),
      'agres_views_entity' => array(
        'title' => t('agres_views Entities'),
        'help' => t('Displays each selected entity as a Calendar item.'),
        'handler' => 'agres_views_plugin_row',
        'path' => "$module_path/includes",
        'uses options' => TRUE,
        'uses fields' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
  return $data;
}