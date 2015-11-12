<?php

/**
 * @file
 * Defines the View Style Plugins for Views Showcase module.
 */

/**
 * Implements hook_views_plugins().
 */
function views_showcase_views_plugins() {
  return array(
    'style' => array(
      'views_showcase' => array(
        'title' => t('Views Showcase'),
        'theme' => 'views_showcase_view',
        'help' => t('Display a view like a showcase.'),
        'handler' => 'views_showcase_plugin_style_showcase',
        'path' => drupal_get_path('module', 'views_showcase'),
        'uses options' => TRUE,
        'uses row plugin' => TRUE,
        'uses grouping' => FALSE,
        'type' => 'normal',
      ),
    ),
  );
}
