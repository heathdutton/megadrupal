<?php

/**
 * @file
 * Plugin file for VBO Hide module
 */

/**
 * Implements hook_views_plugin().
 */
function vbo_hide_views_plugins() {
  return array(
    'display_extender' => array(
      'vbo_hide' => array(
        'title' => t('Hide VBO'),
        'help' => t('Hide the VBO if no data in view.'),
        'path' => drupal_get_path('module', 'vbo_hide') . '/views',
        'handler' => 'vbo_hide_plugin_display_extender',
      ),
    ),
  );
}
