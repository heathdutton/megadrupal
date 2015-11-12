<?php

/**
 * @file
 * Views integration.
 */

/**
 * Implements hook_views_plugins().
 */
function datastore_visualization_views_plugins() {
  return array(
    'module' => 'datastore_visualization',
    'style'  => array(
      'datastore_visualization' => array(
        'handler' => 'datastore_visualization_plugin_style_chart',
        'help' => t('Renders datastores as charts.'),
        'path' => drupal_get_path('module', 'datastore_visualization') . '/views',
        'parent' => 'parent',
        'title' => t('Datastore chart'),
        'type' => 'normal',
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'uses row plugin' => FALSE,
      ),
    )
  );
}
