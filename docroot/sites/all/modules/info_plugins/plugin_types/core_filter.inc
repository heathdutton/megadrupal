<?php

/**
 * Defines the plugin type for the core filter plugin.
 *
 * @param array $items
 */
function info_plugins_core_filter_plugin_type(&$items) {
  $items['core_filter'] = array(
    'label' => 'Core Filter',
    'description' => t('Allows the definition of core Drupal filters as cTools plugins.'),
    'use hooks' => FALSE,
    'process' => 'info_plugins_core_filter_process_plugin'
  );
}

/**
 * Process core_filter plugins.
 *
 * @param array $plugin
 * @param array $info
 */
function info_plugins_core_filter_process_plugin(&$plugin, $info) {
  // Construct a title from the plugin name.
  if (!isset($plugin['title'])) {
    $exploded = explode('_', $plugin['name']);
    $name = '';
    foreach ($exploded as $part) {
      $name .= ucfirst($part) . ' ';
    }
    $plugin['title'] = $name;
  }

  // Add a settings callback.
  if (!isset($plugin['settings callback'])) {
    $plugin['settings callback'] = $plugin['module'] . '_' . $plugin['name'] . '_settings';
  }

  // Add a prepare callback.
  if (!isset($plugin['prepare callback'])) {
    $plugin['prepare callback'] = $plugin['module'] . '_' . $plugin['name'] . '_prepare';
  }

  // Add a process callback.
  if (!isset($plugin['process callback'])) {
    $plugin['process callback'] = $plugin['module'] . '_' . $plugin['name'] . '_process';
  }

  // Add a tips callback.
  if (!isset($plugin['tips callback'])) {
    $plugin['tips callback'] = $plugin['module'] . '_' . $plugin['name'] . '_tips';
  }
}

/**
 * Loads a core_filter plugin by name.
 *
 * @param string $name
 * @return mixed
 */
function info_plugins_core_filter_load($name) {
  ctools_include('plugins');
  return ctools_get_plugins('info_plugins', 'core_filter', $name);
}

/**
 * Implements hook_filter_info().
 */
function info_plugins_filter_info() {
  $return = array();

  ctools_include('plugins');
  $plugins = ctools_get_plugins('info_plugins', 'core_filter');
  if (!$plugins) {
    return;
  }

  foreach ($plugins as $name => $plugin) {
    $return[$name] = info_plugins_filter_info_from_plugin($plugin);
  }

  return $return;
}

/**
 * Builds the array for hook_filter_info().
 *
 * @param array $plugin
 * @return array
 */
function info_plugins_filter_info_from_plugin($plugin) {
  // Required values
  $info = array(
    'title' => $plugin['title'],
    'process callback' => $plugin['process callback'],
  );

  // Optional values
  $keys = array('description', 'settings callback', 'default settings', 'prepare callback', 'cache', 'tips callback', 'weight');
  foreach ($keys as $key) {
    if (isset($plugin[$key])) {
      $info[$key] = $plugin[$key];
    }
  }

  return $info;
}
