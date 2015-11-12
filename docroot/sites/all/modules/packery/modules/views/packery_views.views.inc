<?php

/**
 * @file
 * packery_views.views.inc
 */

/**
 * Implements hook_views_plugins().
 */
function packery_views_views_plugins() {
  return array(
    'style' => array(
      'packery' => array(
        'title' => t('Packery'),
        'help' => t('Displays the view output in a Packery grid'),
        'help topic' => '',
        'handler' => 'packery_views_plugin_style_packery',
        'theme' => 'packery_views',
        'theme file' => 'packery_views.theme.inc',
        'theme path' => drupal_get_path('module', 'packery_views') . '/theme',
        'uses row plugin' => TRUE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'even empty' => FALSE,
      ),
    )
  );
}
