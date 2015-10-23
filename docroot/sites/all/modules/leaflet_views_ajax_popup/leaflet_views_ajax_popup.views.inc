<?php

/**
 * @file
 * Leaflet maps views integration with AJAX popups.
 */

/**
 * Define leaflet views style.
 *
 * Implements hook_views_plugins().
 */
function leaflet_views_ajax_popup_views_plugins() {
  $plugins = array(
    'module' => 'leaflet_views_ajax_popup',
    'style' => array(
      'leaflet_views_ajax_popup' => array(
        'title' => t('Leaflet Map (AJAX Popups)'),
        'help' => t('Displays a View as a Leaflet map with popups loaded via AJAX.'),
        'path' => drupal_get_path('module', 'leaflet_views_ajax_popup'),
        'handler' => 'leaflet_views_ajax_popup_plugin_style',
        'theme' => 'leaflet-map',
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
