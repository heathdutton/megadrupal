<?php

/**
 * Defines the plugin type for the core field formatter plugin.
 *
 * @param array $items
 */
function info_plugins_core_field_formatter_plugin_type(&$items) {
  $items['core_field_formatter'] = array(
    'label' => 'Core Field Formatter',
    'description' => t('Allows the definition of core Drupal field formatters as cTools plugins.'),
    'use hooks' => FALSE,
    'process' => 'info_plugins_core_field_formatter_process_plugin'
  );
}

/**
 * Process core_field_formatter plugins.
 *
 * @param array $plugin
 * @param array $info
 */
function info_plugins_core_field_formatter_process_plugin(&$plugin, $info) {
  // Add a label
  if (!isset($plugin['label'])) {
    $exploded = explode('_', $plugin['name']);
    $name = '';
    foreach ($exploded as $part) {
      $name .= ucfirst($part) . ' ';
    }
    $plugin['label'] = $name;
  }

  // Add a formatter view function
  if (!isset($plugin['view'])) {
    $plugin['view'] = $plugin['module'] . '_' . $plugin['name'] . '_view';
  }

  // Add a formatter setting_form function
  if (!isset($plugin['settings form'])) {
    $plugin['settings form'] = $plugin['module'] . '_' . $plugin['name'] . '_settings_form';
  }

  // Add a formatter settings_summary function
  if (!isset($plugin['settings summary'])) {
    $plugin['settings summary'] = $plugin['module'] . '_' . $plugin['name'] . '_settings_summary';
  }
}

/**
 * Loads a core_field_formatter plugin by name.
 *
 * @param string $name
 * @return mixed
 */
function info_plugins_core_field_formatter_load($name) {
  ctools_include('plugins');
  return ctools_get_plugins('info_plugins', 'core_field_formatter', $name);
}

/**
 * Implements hook_field_formatter_info().
 */
function info_plugins_field_formatter_info() {
  $return = array();

  ctools_include('plugins');
  $plugins = ctools_get_plugins('info_plugins', 'core_field_formatter');
  foreach ($plugins as $plugin) {
    $return[$plugin['name']] = info_plugins_field_formatter_info_from_plugin($plugin);
  }

  return $return;
}

/**
 * Implements hook_field_formatter_view().
 */
function info_plugins_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, &$items, $display) {

  $plugin = info_plugins_core_field_formatter_load($display['type']);
  if (!$plugin) {
    return;
  }

  if (!function_exists($plugin['view'])) {
    return;
  }

  return $plugin['view']($entity_type, $entity, $field, $instance, $langcode, $items, $display);
}


/**
 * Implements hook_field_formatter_settings_form().
 */
function info_plugins_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $formatter = $instance['display'][$view_mode]['type'];
  $plugin = info_plugins_core_field_formatter_load($formatter);
  if (!$plugin) {
    return;
  }

  if (!function_exists($plugin['settings form'])) {
    return;
  }

  return $plugin['settings form']($field, $instance, $view_mode, $form, $form_state);
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function info_plugins_field_formatter_settings_summary($field, $instance, $view_mode) {
  $formatter = $instance['display'][$view_mode]['type'];
  $plugin = info_plugins_core_field_formatter_load($formatter);
  if (!$plugin) {
    return;
  }

  if (!function_exists($plugin['settings summary'])) {
    return;
  }

  return $plugin['settings summary']($field, $instance, $view_mode);
}

/**
 * Builds the array for hook_field_formatter_info().
 *
 * @param $plugin
 * @return array
 */
function info_plugins_field_formatter_info_from_plugin($plugin) {
  $info = array(
    'label' => $plugin['label']
  );

  $keys = array('description', 'field types', 'settings');
  foreach ($keys as $key) {
    if (isset($plugin[$key])) {
      $info[$key] = $plugin[$key];
    }
  }

  return $info;
}