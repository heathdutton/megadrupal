<?php

/**
 * Defines the plugin type for the core action plugin.
 *
 * @param array $items
 */
function info_plugins_core_action_plugin_type(&$items) {
  $items['core_action'] = array(
    'label' => 'Core Action',
    'description' => t('Allows the definition of core Drupal actions as cTools plugins.'),
    'use hooks' => FALSE,
    'process' => 'info_plugins_core_action_process_plugin'
  );
}

/**
 * Process core_action plugins.
 *
 * @param array $plugin
 * @param array $info
 */
function info_plugins_core_action_process_plugin(&$plugin, $info) {
  // Construct a label from the plugin name.
  if (!isset($plugin['label'])) {
    $exploded = explode('_', $plugin['name']);
    $name = '';
    foreach ($exploded as $part) {
      $name .= ucfirst($part) . ' ';
    }
    $plugin['label'] = $name;
  }
}

/**
 * Loads a core_action plugin by name.
 *
 * @param string $name
 * @return mixed
 */
function info_plugins_core_action_load($name) {
  ctools_include('plugins');
  return ctools_get_plugins('info_plugins', 'core_action', $name);
}

/**
 * Implements hook_action_info().
 */
function info_plugins_action_info() {
  $return = array();

  ctools_include('plugins');
  $plugins = ctools_get_plugins('info_plugins', 'core_action');
  if (!$plugins) {
    return;
  }

  foreach ($plugins as $name => $plugin) {
    $return[$name] = info_plugins_action_info_from_plugin($plugin);
  }

  return $return;
}

/**
 * Builds the array for hook_action_info().
 *
 * @param array $plugin
 * @return array
 */
function info_plugins_action_info_from_plugin($plugin) {
  $info = array();

  // Core values.
  $keys = array('type', 'label', 'configurable', 'triggers', 'behaviour');
  foreach ($keys as $key) {
    if (isset($plugin[$key])) {
      $info[$key] = $plugin[$key];
    }
  }

  // VBO values
  $keys = array('vbo_configurable', 'aggregate', 'pass rows', 'permissions');
  foreach ($keys as $key) {
    if (isset($plugin[$key])) {
      $info[$key] = $plugin[$key];
    }
  }

  return $info;
}
