<?php

/**
 * @file
 * D3 maps views integration.
 */

/**
 * Define d3 map views style.
 *
 * Implements hook_views_plugins().
 */
function d3map_views_views_plugins() {
  $plugins = array(
    'module' => 'd3map_views',
    'style' => array(
      'd3map' => array(
        'title' => t('D3 Map'),
        'help' => t('Displays a View as a D3 map.'),
        'path' => drupal_get_path('module', 'd3map_views'),
        'handler' => 'd3map_views_plugin_style',
        'theme' => 'd3-map',
        'uses fields' => TRUE,
        'uses row plugin' => FALSE,
        'uses options' => TRUE,
        'uses grouping' => FALSE,
        'type' => 'normal',
        'even empty' => TRUE,
      ),
    ),
  );

  return $plugins;
}