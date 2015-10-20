<?php
/**
 * @file
 * Implements the display extender to add CSS and JS field to displays.
 */

/**
 * Implements hook_views_plugins
 */
function cpv_views_plugins() {
  $path = drupal_get_path('module', 'cpv');
  $plugins = array();
  $plugins['display_extender']['cpv'] = array(
    'title' => t('Code per Display'),
    'help' => t('Add custom CSS and JS to views display.'),
    'path' => $path,
    'handler' => 'cpv_plugin_display_extender_code',
  );

  return $plugins;
}
