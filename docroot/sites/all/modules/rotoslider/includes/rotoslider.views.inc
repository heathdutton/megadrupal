<?php

/**
 * @file
 * Views integration for Rotoslider.
 */

/**
 * Implements hook_views_plugins().
 */
function rotoslider_views_plugins() {
  $module_path = drupal_get_path('module', 'rotoslider');
  $theme_path = $module_path . '/theme';
  $path = $module_path . '/includes';
  return array(
    'module' => 'rotoslider',
    'style' => array(
      'rotoslider' => array(
        'title' => t('Slider'),
        'help' => t('Displays items in a slider.'),
        'handler' => 'rotoslider_plugin_style_slider',
        'path' => $path,
        'theme' => 'rotoslider_view_slider',
        'theme file' => 'theme.inc',
        'theme path' => $theme_path,
        'uses row plugin' => FALSE,
        'uses grouping' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}
