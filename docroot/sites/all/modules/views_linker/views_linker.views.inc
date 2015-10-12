<?php
/**
 * @file
 * Views integration for the Views Linker module.
 */

/**
 * Implements hook_views_plugins().
 */
function views_linker_views_plugins() {
  $path = drupal_get_path('module', 'views_linker_views');
  $plugins = array();
  $plugins['display_extender']['views_linker'] = array(
    'title' => t('Views Linker'),
    'help' => t('Option to navigate back to a Views listing.'),
    'path' => $path,
    'handler' => 'views_linker_plugin_display_extender_back',
    'uses options' => TRUE,
    'enabled' => TRUE,
  );

  return $plugins;
}
