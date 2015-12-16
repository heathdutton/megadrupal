<?php

/**
 * @file
 * Views integration for the views_field_tooltip module.
 */

/**
 * Implements hook_views_plugins().
 */
function views_field_tooltip_views_plugins() {
  $path = drupal_get_path('module', 'views_field_tooltip') . '/views';
  $plugins = array();
  $plugins['display_extender']['views_field_tooltip'] = array(
    'title' => t('Views Field Tooltip display extender'),
    'path' => $path,
    'handler' => 'views_field_tooltip_plugin_display_extender',
  );
  return $plugins;
}
