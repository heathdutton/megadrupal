<?php

/**
 * Defines the plugin type for the core field_widget plugin.
 *
 * @param array $items
 */
function info_plugins_core_field_widget_plugin_type(&$items) {
  $items['core_field_widget'] = array(
    'label' => 'Core Field Widget',
    'description' => t('Allows the definition of core Drupal field widgets as cTools plugins.'),
    'use hooks' => FALSE,
    'process' => 'info_plugins_core_field_widget_process_plugin'
  );
}

/**
 * Process core_field_widget plugins.
 *
 * @param array $plugin
 * @param array $info
 */
function info_plugins_core_field_widget_process_plugin(&$plugin, $info) {
  // Construct a label from the plugin name.
  if (!isset($plugin['label'])) {
    $exploded = explode('_', $plugin['name']);
    $name = '';
    foreach ($exploded as $part) {
      $name .= ucfirst($part) . ' ';
    }
    $plugin['label'] = $name;
  }

  // Add a form callback.
  if (!isset($plugin['form'])) {
    $plugin['form'] = $plugin['module'] . '_' . $plugin['name'] . '_form';
  }

  // Add a settings form callback.
  if (!isset($plugin['settings form'])) {
    $plugin['settings form'] = $plugin['module'] . '_' . $plugin['name'] . '_settings';
  }
}

/**
 * Loads a core_field_widget plugin by name.
 *
 * @param string $name
 * @return mixed
 */
function info_plugins_core_field_widget_load($name) {
  ctools_include('plugins');
  return ctools_get_plugins('info_plugins', 'core_field_widget', $name);
}

/**
 * Implements hook_field_widget_info().
 */
function info_plugins_field_widget_info() {
  $return = array();

  ctools_include('plugins');
  $plugins = ctools_get_plugins('info_plugins', 'core_field_widget');
  if (!$plugins) {
    return;
  }

  foreach ($plugins as $name => $plugin) {
    $return[$name] = info_plugins_field_widget_info_from_plugin($plugin);
  }

  return $return;
}

/**
 * Builds the array for hook_field_widget_info().
 *
 * @param array $plugin
 * @return array
 */
function info_plugins_field_widget_info_from_plugin($plugin) {
  $info = array();

  // All values
  $keys = array('label', 'description', 'field types', 'settings', 'behaviours', 'weight');
  foreach ($keys as $key) {
    if (isset($plugin[$key])) {
      $info[$key] = $plugin[$key];
    }
  }

  return $info;
}

/**
 * Implements hook_field_widget_form().
 */
function info_plugins_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $widget = $instance['widget']['type'];
  $plugin = info_plugins_core_field_widget_load($widget);
  if (!$plugin) {
    return;
  }

  if (!function_exists($plugin['form'])) {
    return;
  }

  return $plugin['form']($form, $form_state, $field, $instance, $langcode, $items, $delta, $element);
}

/**
 * Implements hook_field_widget_error().
 */
function info_plugins_field_widget_error($element, $error, $form, &$form_state) {
  form_error($element['value'], $error['message']);
}

/**
 * Implements hook_field_widget_settings_form().
 */
function info_plugins_field_widget_settings_form($field, $instance) {
  $widget = $instance['widget']['type'];
  $plugin = info_plugins_core_field_widget_load($widget);
  if (!$plugin) {
    return;
  }

  if (!function_exists($plugin['settings form'])) {
    return;
  }

  return $plugin['settings form']($field, $instance);
}