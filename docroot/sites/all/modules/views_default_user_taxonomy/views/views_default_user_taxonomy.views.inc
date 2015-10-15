<?php
/**
 * @file
 */

/**
 * Implements hook_views_plugins_alter().
 */
function views_default_user_taxonomy_views_plugins_alter(&$plugins) {
  $plugins['argument default']['taxonomy_tid']['handler'] = 'views_plugin_argument_default_user_taxonomy_tid';
  $plugins['argument default']['taxonomy_tid']['path'] = drupal_get_path('module', 'views_default_user_taxonomy') . '/views/plugins';
  $plugins['argument default']['taxonomy_tid']['file'] = 'views_plugin_argument_default_user_taxonomy_tid.inc';
}
