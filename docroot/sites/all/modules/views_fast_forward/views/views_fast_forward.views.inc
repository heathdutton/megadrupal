<?php

/**
 * @file
 * The base Views plugin bootstrapper.
 *
 * Here we only load one plugin, a Display Extender to
 * provide extra options in the Views "advanced" sections.
 */

/**
 * Implements hook_views_style_plugins().
 */
function views_fast_forward_views_plugins() {
  return array(
    'display_extender' => array(
      'views_fast_forward' => array(
        'title' => t('Views Fast Forward'),
        'help' => t('If this view only outputs one result, redirect to a specific path.'),
        'path' => drupal_get_path('module', 'views_fast_forward') . '/views',
        'handler' => 'views_fast_forward_plugin_display_extender',
      ),
    ),
  );
}
