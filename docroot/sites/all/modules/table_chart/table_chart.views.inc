<?php

/**
 * @file
 * Views integration for the table_chart module.
 */

/**
 * Implements hook_views_plugins().
 */
function table_chart_views_plugins() {
  $plugins = array();
  $plugins['display_extender']['attributes'] = array(
    'title' => t('Attributes'),
    'help' => t('Allows attributes to be added to views.'),
    'handler' => 'table_chart_plugin_display_extender_attributes',
    'enabled' => TRUE,
  );

  return $plugins;
}