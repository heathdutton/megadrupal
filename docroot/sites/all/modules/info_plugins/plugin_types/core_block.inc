<?php

/**
 * Defines the plugin type for the core block plugin.
 *
 * @param array $items
 */
function info_plugins_core_block_plugin_type(&$items) {
  $items['core_block'] = array(
    'label' => 'Core Block',
    'description' => t('Allows the definition of core Drupal blocks as cTools plugins.'),
    'use hooks' => FALSE,
    'process' => 'info_plugins_core_block_process_plugin'
  );
}

/**
 * Process core_block plugins.
 *
 * @param array $plugin
 * @param array $info
 */
function info_plugins_core_block_process_plugin(&$plugin, $info) {
  // Add a block admin title
  if (!isset($plugin['admin title'])) {
    $exploded = explode('_', $plugin['name']);
    $name = '';
    foreach ($exploded as $part) {
      $name .= ucfirst($part) . ' ';
    }
    $plugin['admin title'] = $name;
  }

  // By default we also show a block title but this can be overwritten
  if (!isset($plugin['show title'])) {
    $plugin['show title'] = TRUE;
  }

  // Add a block view function
  if (!isset($plugin['view'])) {
    $plugin['view'] = $plugin['module'] . '_' . $plugin['name'] . '_view';
  }

  // Add a block form function
  if (!isset($plugin['configure'])) {
    $plugin['configure'] = $plugin['module'] . '_' . $plugin['name'] . '_configure';
  }

  // Add a block save function
  if (!isset($plugin['save'])) {
    $plugin['save'] = $plugin['module'] . '_' . $plugin['name'] . '_save';
  }
}

/**
 * Loads a core_block plugin by name.
 *
 * @param string $name
 * @return mixed
 */
function info_plugins_core_block_load($name) {
  ctools_include('plugins');
  return ctools_get_plugins('info_plugins', 'core_block', $name);
}

/**
 * Implements hook_block_info().
 */
function info_plugins_block_info() {
  $blocks = array();

  ctools_include('plugins');
  $plugins = ctools_get_plugins('info_plugins', 'core_block');
  foreach ($plugins as $name => $plugin) {
    $blocks[$name] = info_plugins_block_info_from_plugin($plugin);
  }

  return $blocks;
}

/**
 * Builds the array for hook_block_info().
 *
 * @param array $plugin
 * @return array
 */
function info_plugins_block_info_from_plugin($plugin) {
  // Required values
  $info = array(
    'info' => $plugin['admin title'],
  );

  // Optional values
  $keys = array('cache', 'properties', 'weight', 'status', 'region', 'visibility', 'pages');
  foreach ($keys as $key) {
    if (isset($plugin[$key])) {
      $info[$key] = $plugin[$key];
    }
  }

  return $info;
}

/**
 * Implements hook_block_view().
 */
function info_plugins_block_view($delta = '') {
  $plugin = info_plugins_core_block_load($delta);
  if (!$plugin) {
    return;
  }

  if (!function_exists($plugin['view'])) {
    return;
  }

  $block = array();

  // Optional title
  if (isset($plugin['title']) && $plugin['show title'] !== FALSE) {
    $block['subject'] = $plugin['title'];
  }

  // Block content
  $block['content'] = $plugin['view']($delta);

  return $block;
}

/**
 * Implements hook_block_configure().
 */
function info_plugins_block_configure($delta = '') {
  $plugin = info_plugins_core_block_load($delta);
  if (!$plugin) {
    return;
  }

  if (!function_exists($plugin['configure'])) {
    return;
  }

  return $plugin['configure']($delta);
}

/**
 * Implements hook_block_save().
 */
function info_plugins_block_save($delta = '', $edit = array()) {
  $plugin = info_plugins_core_block_load($delta);
  if (!$plugin) {
    return;
  }

  if (!function_exists($plugin['save'])) {
    return;
  }

  return $plugin['save']($edit);
}