<?php

/**
 * @file
 * Content for Node Subpages: Field values
 */

// Plugin details
$plugin = array(
  'title' => t('Field'),
  'description' => t("Use a Field's value as the content for the subpage"),
  'dependencies' => array('field'),
  'config form' => 'node_subpages_content_field_config',
  'config save prep' => 'node_subpages_content_field_config_save_prep',
  'config summary' => 'node_subpages_content_field_config_summary',
  'content' => 'node_subpages_content_field_content',
  'has content' => 'node_subpages_content_field_has_content',
);

/**
 * Config form for the subpages using field content
 */
function node_subpages_content_field_config($type, $plugin_config) {
  $options = array('' => '-- Select One --');
  $formatters = array();
  $fields = field_info_instances('node', $type->type);
  if ($fields) {
    foreach ($fields as $field_key => $field_details) {
      $options[$field_key] = $field_details['label'] . ' (' . $field_key . ')';
      $formatters[$field_key] = _node_subpages_content_field_formatter_options($field_key);
    }
  }

  $form = array(
    '#tree' => TRUE,
  );
  $form['field'] = array(
    '#type' => 'select',
    '#title' => t('Field'),
    '#description' => t('Field to use as the content of this subpage.'),
    '#multiple' => FALSE,
    '#options' => $options,
    '#default_value' => isset($plugin_config['field']) ? $plugin_config['field'] : '',
  );

  // Add dropdowns for the formatter for the field, one per field that can be
  // chosen. The dropdown will be switched based on the chosen field.
  foreach ($formatters as $field_key => $formatter_options) {
    if (!empty($formatter_options)) {
      // If this subpage already has a formatter chosen
      if (isset($plugin_config['formatter'])) {
        $default_value = $plugin_config['formatter'];
      }
      // If not, check the default formatter for the field
      else {
        $default_value = isset($fields[$field_key]['display']['default']['type']) ? $fields[$field_key]['display']['default']['type'] : NULL;
      }

      $form[$field_key]['formatter'] = array(
        '#type' => 'select',
        '#title' => t('Field Formatter'),
        '#description' => t('Formatter to use for ') . $field_key,
        '#multiple' => FALSE,
        '#options' => $formatter_options,
        '#default_value' => $default_value,
        '#states' => array(
          'visible' => array(
            ':input[name="options[field]"]' => array('value' => $field_key),
          ),
        ),
      );

      // Add the options for each formatter
      foreach ($formatter_options as $formatter_key => $formatter_label) {
        $config_form = _node_subpages_content_field_formatter_config_form($formatter_key, $fields[$field_key], $plugin_config);
        if (!empty($config_form)) {
          // Figure out the name of the formatter dropdown
          $parent_element_name = 'options[' . $field_key . '][formatter]';
          // For each of the formatter options:
          foreach (element_children($config_form) as $k) {
            // Don't make it required
            $config_form[$k]['#required'] = FALSE;
            // Don't show it unless this formatter is chosen
            $config_form[$k]['#states'] = array(
              'visible' => array(
                ':input[name="' . $parent_element_name . '"]' => array('value' => $formatter_key),
                // Also make sure that this field is chosen (otherwise, this
                // option might be shown when the hidden formatter dropdowns are
                // set to this formatter)
                ':input[name="options[field]"]' => array('value' => $field_key),
              ),
            );
            // Update the default value
          }
          $form[$field_key][$formatter_key] = $config_form;
        }
      }
    }
  }

  return $form;
}


/**
 * Prep the config for save to the DB, by placing into an array
 */
function node_subpages_content_field_config_save_prep($values) {
  $field_name = $values['options']['field'];
  $formatter = $values['options'][$field_name]['formatter'];
  $formatter_options = $values['options'][$field_name][$formatter];
  return array(
    'field' => $field_name,
    'formatter' => $formatter,
    'formatter_options' => $formatter_options,
  );
}


/**
 * Return the content for a subpage for the given node and plugin config
 * Should this use field_attach_view? See node_build_content().
 */
function node_subpages_content_field_content($node, $plugin_config) {
  // Build display options
  $display = array(
    // Don't show the field label
    'label' => 'hidden',
  );

  // Use the chosen formatter
  if (isset($plugin_config['formatter'])) {
    $display['type'] = $plugin_config['formatter'];

    // Get the formatter in use here
    $all_formatters = field_info_formatter_types();
    $formatter = $all_formatters[$plugin_config['formatter']];

    // Combine the default settings for the formatter with the chosen options
    $settings = $formatter['settings'];
    if (isset($plugin_config['formatter_options'])) {
      $settings = array_merge($settings, $plugin_config['formatter_options']);
    }
    $display['settings'] = $settings;
  }

  $field = field_view_field('node', $node, $plugin_config['field'], $display);
  return $field;
}


/**
 * Check if the subpage will have any content
 */
function node_subpages_content_field_has_content($node, $plugin_config) {
  $items = field_get_items('node', $node, $plugin_config['field']);
  return !empty($items);
}


/**
 * Return a summary for the configuration
 *
 * @param $plugin_config
 * Array of config options for the plugin
 *
 * @param $details
 * Array of full details, including subpath, plugin, etc
 */
function node_subpages_content_field_config_summary($plugin_config, $details) {
  // Get all fields for this node type
  $fields = field_info_instances('node', $details['node_type']);
  // Get the chosen field
  $field = $fields[$plugin_config['field']];
  // Get the chosen formatter
  $formatter = isset($plugin_config['formatter']) ? $plugin_config['formatter'] : NULL;
  $field_formatter_label = _node_subpages_content_field_formatter_label($plugin_config['field'], $formatter);

  // Build a summary
  if ($field_formatter_label) {
    return t('Field: %field_name, Format: %formatter', array('%field_name' =>  $field['label'], '%formatter' => $field_formatter_label));
  }
  else {
    return t('Field: %field_name', array('%field_name' =>  $field['label']));
  }
}


/**
 * Helper function: get a list of formatters available for a given field
 */
function _node_subpages_content_field_formatter_options($field_name) {
  $formatters = &drupal_static(__FUNCTION__);

  if (!isset($formatters[$field_name])) {
    module_load_include('inc', 'field_ui', 'field_ui.admin');
    $field = field_info_field($field_name);
    $formatters[$field_name] = field_ui_formatter_options($field['type']);
  }
  return $formatters[$field_name];
}


/**
 * Helper function: get the lable for a formatter on a given field
 */
function _node_subpages_content_field_formatter_label($field_name, $formatter) {
  $formatters = _node_subpages_content_field_formatter_options($field_name);
  if ($formatters && isset($formatters[$formatter])) {
    return $formatters[$formatter];
  }
  return '';
}


/**
 * Get the config form for a field formatter.
 */
function _node_subpages_content_field_formatter_config_form($formatter_key, $instance, $plugin_config = array()) {
  $config_forms = &drupal_static(__FUNCTION__);
  $all_formatters = &drupal_static(__FUNCTION__ . '_formatters');

  if (empty($all_formatters)) {
    $all_formatters = field_info_formatter_types();
  }

  if (!isset($config_forms[$formatter_key])) {
    $config_forms[$formatter_key] = array();

    // Get the formatter details
    $formatter = $all_formatters[$formatter_key];
    // See if the module providing the formatter has a
    // hook__field_formatter_settings_form()
    $function = $formatter['module'] . '_field_formatter_settings_form';
    if (function_exists($function)) {
      // Get the full field info
      $field = field_info_field($instance['field_name']);
      // Junk vars that the _field_formatter_settings_form function needs
      $form = $form_state = array();
      $view_mode = 'default';
      // Force the formatter type on the field instance to the one we're
      // interested in.
      $instance['display'][$view_mode]['type'] = $formatter_key;

      // Set any other settings from the plugin, but only if the plugin config
      // has options for the formatter key in question
      // Use the defaults from the formatter to start with.
      $settings = $formatter['settings'];
      if (isset($plugin_config['formatter']) &&
          $formatter_key == $plugin_config['formatter'] &&
          isset($plugin_config['formatter_options'])) {
        $settings = array_merge($settings, $plugin_config['formatter_options']);
      }
      $instance['display'][$view_mode]['settings'] = $settings;

      // Get the form
      $config_forms[$formatter_key] = $function($field, $instance, $view_mode, $form, $form_state);
    }
  }

  return $config_forms[$formatter_key];
}
