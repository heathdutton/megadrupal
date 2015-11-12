<?php
/**
 * @file
 * Views hooks for Views Elastic Grid
 */

/**
 * Implements hook_views_plugins().
 */
function views_elastic_grid_views_plugins() {
  $path = drupal_get_path('module', 'views_elastic_grid') . '/plugins';

  $plugins = array(
    'style' => array(
      'views_elastic_grid' => array(
        'title' => t('Elastic grid'),
        'help' => t('Displays a grid of thumbnails with expandable previews.'),
        'handler' => 'views_elastic_grid_plugin_style_elastic_grid',
        'theme' => 'views_elastic_grid_list',
        'uses row plugin' => FALSE,
        'uses fields' => TRUE,
        'uses row class' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'path' => $path,
      ),
    ),
  );

  return $plugins;
}