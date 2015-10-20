<?php

/**
 * Returns an array of all the plugin types.
 *
 * @return array
 */
function info_plugins_get_all_plugin_types() {
  $items = array();
  $files = file_scan_directory(drupal_get_path('module', 'info_plugins') . '/plugin_types', '/.*\.inc$/', array('key' => 'name'));
  foreach ($files as $file) {
    require_once DRUPAL_ROOT . '/' . $file->uri;

    $function = 'info_plugins' . '_' . str_replace ('-', '_', $file->name) . '_plugin_type';
    if (function_exists($function)) {
      $function($items);
    }
  }

  return $items;
}

/**
 * Returns an array of the enabled plugin types.
 *
 * @return array
 */
function info_plugins_get_enabled_plugin_types() {
  $enabled = variable_get('info_plugins_enabled_types', array());
  $items = array();
  foreach ($enabled as $name) {
    require_once DRUPAL_ROOT . '/' . drupal_get_path('module', 'info_plugins') . '/plugin_types/' . $name . '.inc';
    $function = 'info_plugins' . '_' . str_replace ('-', '_', $name) . '_plugin_type';
    if (function_exists($function)) {
      $function($items);
    }
  }
  return $items;
}
