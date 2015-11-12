<?php

/**
 * @file
 * Views inc file.
 */

/**
 * Implements hook_views_plugins().
 */
function views_lazy_load_views_plugins() {
  return array(
    'module' => 'views_lazy_load',
    'display_extender' => array(
      'views_lazy_load' => array(
        'title' => t('Views Lazy Load'),
        'help' => t('Lazy loads the view via AJAX'),
        'path' => drupal_get_path('module', 'views_lazy_load') . '/includes/views',
        'handler' => 'views_lazy_load_plugin_display_extender',
      ),
    ),
  );
}
