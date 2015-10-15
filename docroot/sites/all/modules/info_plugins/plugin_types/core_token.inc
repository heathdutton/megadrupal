<?php

/**
 * Defines the plugin type for the core token plugin.
 *
 * @param array $items
 */
function info_plugins_core_token_plugin_type(&$items) {
  $items['core_token'] = array(
    'label' => 'Core Token',
    'description' => t('Allows the definition of core Drupal tokens as cTools plugins.'),
    'use hooks' => FALSE,
    'process' => 'info_plugins_core_token_process_plugin'
  );
}

/**
 * Process core_token plugins.
 *
 * @param array $plugin
 * @param array $info
 */
function info_plugins_core_token_process_plugin(&$plugin, $info) {

  // Add a default type
  if (!isset($plugin['type machine_name'])) {
    $type = info_plugins_core_token_get_default_type();
    $plugin += $type;
  }

  // Add a default callback
  if (!isset($plugin['callback'])) {
    $plugin['callback'] = $plugin['module'] . '_' . $plugin['name'] . '_token';
  }

}

/**
 * Loads a core_token plugin by name.
 *
 * @param string $name
 * @return mixed
 */
function info_plugins_core_token_load($name) {
  ctools_include('plugins');
  return ctools_get_plugins('info_plugins', 'core_token', $name);
}

/**
 * Loads all the core_token plugins and returns them in an associated array
 * keyed by their type.
 *
 * @return array
 */
function info_plugins_core_token_load_all() {

  $tokens = &drupal_static(__FUNCTION__);
  if ($tokens) {
    return $tokens;
  }

  $tokens = array();
  ctools_include('plugins');
  $plugins = ctools_get_plugins('info_plugins', 'core_token');
  foreach ($plugins as $name => $plugin) {
    if (!isset($tokens[$plugin['type machine_name']])) {
      $tokens[$plugin['type machine_name']] = array($name => $plugin);
      continue;
    }

    $tokens[$plugin['type machine_name']][$name] = $plugin;
  }

  return $tokens;
}

/**
 * Implements hook_token_info().
 */
function info_plugins_token_info() {

  ctools_include('plugins');
  $plugins = ctools_get_plugins('info_plugins', 'core_token');
  $tokens = array();
  $types = array();

  foreach ($plugins as $name => $plugin) {

    // If the token plugin has its own type, set it here. We don't overwrite
    // an existing type if one plugin has already defined it.
    if (!isset($types[$plugin['type machine_name']])) {
      $types[$plugin['type machine_name']] = array(
        'name' => $plugin['type name'],
        'description' => $plugin['type description']
      );
    }

    // Set also the token
    if (!isset($tokens[$plugin['type machine_name']])) {
      $tokens[$plugin['type machine_name']] = array($name => info_plugins_token_info_from_plugin($plugin));
      continue;
    }
    $tokens[$plugin['type machine_name']][$name] = info_plugins_token_info_from_plugin($plugin);
  }

  return array(
    'types' => $types,
    'tokens' => $tokens,
  );
}

/**
 * Implements hook_tokens().
 */
function info_plugins_tokens($type, $tokens, array $data = array(), array $options = array()) {

  $plugins = info_plugins_core_token_load_all();
  if (!$plugins) {
    return;
  }

  if (!isset($plugins[$type])) {
    return;
  }

  $return = array();
  foreach ($tokens as $name => $original) {
    if (!isset($plugins[$type][$name])) {
      continue;
    }
    $plugin = $plugins[$type][$name];
    if (!isset($plugin['callback'])) {
      return;
    }

    // Call the token callback.
    if (function_exists($plugin['callback'])) {
      $return[$original] = $plugin['callback']($data, $options);
    }
  }

  return $return;
}

/**
 * Returns the _info data for a core_token plugin.
 *
 * @param $plugin
 * @return array
 */
function info_plugins_token_info_from_plugin($plugin) {
  $info = array();
  $key_map = array('token name' => 'name', 'token description' => 'description', 'token type' => 'type');
  foreach ($key_map as $key => $value) {
    if (isset($plugin[$key])) {
      $info[$key_map[$key]] = $plugin[$key];
    }
  }

  return $info;
}

/**
 * If token plugins do not set a type, we'll use a default one. This function
 * returns this default.
 *
 * @return array
 */
function info_plugins_core_token_get_default_type() {
  return array(
    'type machine_name' => 'info_plugins',
    'type name' => t('Info plugins'),
    'type description' => t('Tokens provided via the Info Plugins module'),
  );
}
