<?php
/**
 * @file
 * Views plugin defenition.
 *
 */

/**
 * Implements hook_views_plugins().
 */
function views_flag_refresh_views_plugins() {
  $path = drupal_get_path('module', 'views_flag_refresh');
  $plugins = array();
  $plugins['display_extender']['views_flag_refresh'] = array(
    'title' => t('Refresh view by Flag'),
    'path' => $path,
    'handler' => 'views_flag_refresh_plugin_display_extender',
  );
  return $plugins;
}
