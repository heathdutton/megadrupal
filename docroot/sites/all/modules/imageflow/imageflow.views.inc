<?php
/**
 * @file
 * Contains core functions for the Views module support.
 */

/**
 * Implements hook_views_plugins().
 *
 * This function announces the style plugin for imageflow views.
 */
function imageflow_views_plugins() {
  return array(
    'module' => 'imageflow',
    'style' => array(
      // Style plugin for the navigation.
      'imageflow' => array(
        'title' => t('imageflow'),
        'help' => t('Display the results as a imageflow image gallery.'),
        'handler' => 'views_plugin_style_imageflow',
        'theme' => 'views_view_imageflow',
        'theme file' => 'theme.inc',
        'theme path' => drupal_get_path('module', 'imageflow') . '/theme',
        'uses row plugin' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'even empty' => FALSE,
      ),
    ),
  );
}
