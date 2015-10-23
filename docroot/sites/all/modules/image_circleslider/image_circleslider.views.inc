<?php

/**
 * @file
 * Defines the TinyCircleSlider plugin for the views module.
 */

/**
 * Implements the hooks_views_plugins().
 */
function image_circleslider_views_plugins() {

  drupal_add_js('sites/all/libraries/imagecirclesliderlib/js/jquery.tinycircleslider.js');
  drupal_add_js((drupal_get_path('module', 'image_circleslider') . '/js/slider.js'), 'external');
  drupal_add_css((drupal_get_path('module', 'image_circleslider') . '/css/circleslider.css'), array('type' => 'external'));
  return array(
    'module' => 'image_circleslider',
    'style' => array(
      'image_circleslider' => array(
        'title' => t('Image CircleSlider'),
        'help' => t('Display the results as a circleslider.'),
        'handler' => 'views_plugin_style_image_circleslider',
        'theme' => 'image_circleslider_view',
        'uses fields' => TRUE,
        'uses row plugin' => TRUE,
        'type' => 'normal',
        'parent' => 'default',
        'path' => drupal_get_path('module', 'image_circleslider'),
      ),
    ),
  );
}
