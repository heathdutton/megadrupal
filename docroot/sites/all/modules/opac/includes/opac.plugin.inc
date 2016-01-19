<?php

/**
 * @file
 * Manage opac plugins.
 */

/**
 * Require plugin file if needed.
 */
function opac_plugin_require_file($plugin) {
  if ($plugin && isset($plugin['file'])) {
    if (isset($plugin['file path']) && drupal_strlen($plugin['file path']) > 0) {
      $file = $plugin['file path'] . '/' . $plugin['file'];
    }
    else {
      $file = DRUPAL_ROOT . '/' . drupal_get_path('module', $plugin['module'])
        . '/' . $plugin['file'];
    }
    require_once $file;
  }
}

/**
 * Get plugin informations.
 */
function opac_plugin_get($name) {
  $plugins = opac_plugin_get_all();
  $plugin = NULL;
  if ($name && array_key_exists($name, $plugins)) {
    $plugin = $plugins[$name];
  }
  return $plugin;
}

/**
 * Get all plugins.
 */
function opac_plugin_get_all() {
  $plugins = &drupal_static(__FUNCTION__);

  if (!isset($plugins)) {
    $plugins = array();
    $plugins_cache = cache_get('opac_node_field_plugins');
    if ($plugins_cache) {
      $plugins = $plugins_cache->data;
    }
    else {
      $hook = 'opac_node_field_plugin_info';
      $modules = module_implements($hook);
      foreach ($modules as $module) {
        $module_plugins = module_invoke($module, $hook);
        foreach ($module_plugins as &$plugin) {
          $plugin['module'] = $module;
        }
        $plugins = array_merge($plugins, $module_plugins);
      }
      cache_set('opac_node_field_plugins', $plugins);
    }
  }

  return $plugins;
}
