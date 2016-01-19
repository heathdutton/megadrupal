<?php
/**
 * @file
 * Contains core functions for the Views module support.
 */

/*
 * Implements hook_views_plugins().
 */
function photowall_views_plugins() {
  return array(
    'module' => 'photowall',
    'style' => array(
      'photowall' => array(
        'title' => t('Photowall'),
        'help' => t('Display the results using photowall formatter.'),
        'handler' => 'views_plugin_style_photowall',
        'theme' => 'views_view_photowall',
        'theme file' => 'theme.inc',
        'theme path' => drupal_get_path('module', 'photowall') . '/theme',
        'uses row plugin' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'even empty' => FALSE,
      ),
    ),
  );
}
