<?php

/**
 * @file
 * Defines the View Style Plugins for views_gantt module.
 */

/**
 * Implements hook_views_plugins().
 */
function views_gantt_views_plugins() {
  return array(
    'style' => array(
      'gantt' => array(
        'title' => t('Gantt chart'),
        'help' => t('Display the results as a Gantt chart.'),
        'handler' => 'views_gantt_plugin_style_gantt',
        'uses options' => TRUE,
        'uses fields' => TRUE,
        'uses row plugin' => FALSE,
        'uses grouping' => FALSE,
        'uses row class' => TRUE,
        'type' => 'normal',
        'parent' => 'list',
        'theme' => 'views_view_views_gantt',
        'path' => drupal_get_path('module', 'views_gantt'),
      ),
    ),
  );
}
