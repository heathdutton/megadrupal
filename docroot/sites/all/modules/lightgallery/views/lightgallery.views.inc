<?php

/**
 * @file
 * Lightgallery views hooks.
 */

/**
 * Implements hook_views_plugins().
 */
function lightgallery_views_plugins() {
  $path = drupal_get_path('module', 'lightgallery') . '/views';
  return array(
    'module' => 'lightgallery',
    'style' => array(
      'lightgallery' => array(
        'title' => t('Lightgallery'),
        'help' => t('Displays a view as a Lightgallery, using the Lightgallery jQuery plugin.'),
        'path' => $path,
        'theme path' => $path,
        'handler' => 'lightgallery_style_plugin',
        'theme' => 'lightgallery_view',
        'uses row plugin' => FALSE,
        'uses row class' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}
