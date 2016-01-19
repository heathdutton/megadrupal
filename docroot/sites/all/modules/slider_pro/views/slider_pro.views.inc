<?php

/**
 * @file
 * Slider Pro views hooks.
 */

/**
 * Implements hook_views_plugins().
 */
function slider_pro_views_plugins() {
  $path = drupal_get_path('module', 'slider_pro') . '/views';
  return array(
    'module' => 'slider_pro',
    'style' => array(
      'slider_pro' => array(
        'title' => t('Slider Pro'),
        'help' => t('Displays a view as a Slider Pro, using the jQuery plugin Slider Pro.'),
        'path' => $path,
        'theme path' => $path,
        'handler' => 'slider_pro_style_plugin',
        'theme' => 'slider_pro_view',
        'uses row plugin' => FALSE,
        'uses row class' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}
