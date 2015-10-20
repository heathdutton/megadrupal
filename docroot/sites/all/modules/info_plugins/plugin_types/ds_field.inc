<?php

/**
 * Defines the plugin type for the Display Suite field plugin.
 *
 * @param array $items
 */
function info_plugins_ds_field_plugin_type(&$items) {
  if (!module_exists('ds')) {
    return;
  }

  $items['ds_field'] = array(
    'label' => 'Display Suite field',
    'description' => t('Allows the definition of DS fields as cTools plugins.'),
    'use hooks' => FALSE,
    'process' => 'info_plugins_ds_field_process_plugin',
    'provider' => 'ds',
  );
}

/**
 * Process ds_field plugins.
 *
 * @param array $plugin
 * @param array $info
 */
function info_plugins_ds_field_process_plugin(&$plugin, $info) {
  // Construct a title from the plugin name.
  if (!isset($plugin['title'])) {
    $exploded = explode('_', $plugin['name']);
    $name = '';
    foreach ($exploded as $part) {
      $name .= ucfirst($part) . ' ';
    }
    $plugin['title'] = $name;
  }

  // Set the default entity type
  if (!isset($plugin['entity_type'])) {
    $plugin['entity_type'] = 'node';
  }

  // Set the default field type
  if (!isset($plugin['field_type'])) {
    $plugin['field_type'] = DS_FIELD_TYPE_FUNCTION;
  }

  // Set the default render function
  if (!isset($plugin['function']) && $plugin['field_type'] === DS_FIELD_TYPE_FUNCTION) {
    $plugin['function'] = $plugin['module'] . '_' . $plugin['name'] . '_ds_render';
  }

  // Set the default settings summary form function
  if (!isset($plugin['settings form'])) {
    $plugin['settings form'] = $plugin['module'] . '_' . $plugin['name'] . '_settings_form';
  }

  // Set the default format summary function
  if (!isset($plugin['format summary'])) {
    $plugin['format summary'] = $plugin['module'] . '_' . $plugin['name'] . '_format_summary';
  }

  //  Set the default file.
  // If the 'file' key is the cTools default, use that to build the file_path
  if ($plugin['file'] === $plugin['name'] . '.inc') {
    $plugin['file_include'] = $plugin['path'] . '/' . $plugin['file'];
  }
  // Otherwise, use that value directly and set back the original 'file' key
  else {
    $plugin['file_include'] = $plugin['file'];
    $plugin['file'] = $plugin['name'] . '.inc';
  }

  // Check if the 'properties' key is a function and call it for the properties
  if (isset($plugin['properties']) && !is_array($plugin['properties']) && function_exists($plugin['properties'])) {
    $plugin['properties'] = $plugin['properties']();
  }
}

/**
 * Loads a ds_field plugin by name.
 *
 * @param string $name
 * @return mixed
 */
function info_plugins_ds_field_load($name) {
  ctools_include('plugins');
  return ctools_get_plugins('info_plugins', 'ds_field', $name);
}

/**
 * Implements hook_action_info().
 */
function info_plugins_ds_fields_info() {
  $return = array();

  ctools_include('plugins');
  $plugins = ctools_get_plugins('info_plugins', 'ds_field');
  if (!$plugins) {
    return;
  }

  // Group all the plugins by the entity type they work with.
  $grouped_plugins = info_plugins_ds_field_by_entity_type($plugins);

  // Run through all the plugins, get their info and return the array keyed by
  // entity type.
  foreach ($grouped_plugins as $entity_type => $plugins) {
    foreach ($plugins as $plugin) {
      $field = info_plugins_ds_field_info_from_plugin($plugin);
      if (!isset($return[$entity_type])) {
        $return[$entity_type] = array($plugin['name'] => $field);
        continue;
      }

      $return[$entity_type][$plugin['name']] = $field;
    }
  }

  return $return;
}

/**
 * Implements hook_ds_field_settings_form().
 */
function info_plugins_ds_field_settings_form($field) {
  $plugin = info_plugins_ds_field_load($field['name']);
  if (!$plugin) {
    return;
  }

  if (function_exists($plugin['settings form'])) {
    return $plugin['settings form']($field);
  }
}

/**
 * Implements hook_ds_field_format_summary().
 */
function info_plugins_ds_field_format_summary($field) {
  $plugin = info_plugins_ds_field_load($field['name']);
  if (!$plugin) {
    return;
  }

  if (function_exists($plugin['format summary'])) {
    return $plugin['format summary']($field);
  }
}

/**
 * Groups all the ds_field plugins by their entity type.
 *
 * @param array $plugins
 * @return array array
 */
function info_plugins_ds_field_by_entity_type($plugins) {
  $fields = array();
  foreach ($plugins as $plugin) {
    if (!isset($fields[$plugin['entity_type']])) {
      $fields[$plugin['entity_type']] = array($plugin['name'] => $plugin);
      continue;
    }

    $fields[$plugin['entity_type']][$plugin['name']] = $plugin;
  }

  return $fields;
}

/**
 * Builds the array for one field expected in hook_ds_field_info().
 *
 * @param array $plugin
 * @return array
 */
function info_plugins_ds_field_info_from_plugin($plugin) {
  $info = array();

  $keys = array('title', 'field_type', 'ui_limit', 'function', 'properties');
  foreach ($keys as $key) {
    if (isset($plugin[$key])) {
      $info[$key] = $plugin[$key];
    }
  }

  // A bit of break from the norm.
  // @see info_plugins_ds_field_process_plugin() why.
  if (isset($plugin['file_include'])) {
    $info['file'] = $plugin['file_include'];
  }

  return $info;
}
