<?php

$plugin = array(
  'title' => t('My example field'),
  // The entity type this field should work on. Defaults to node.
  'entity_type' => 'node',
  // Defaults to DS_FIELD_TYPE_FUNCTION so you can leave it out if you want
  'field_type' => DS_FIELD_TYPE_FUNCTION,
  'ui_limit' => array('article|default'),
  // An optional file name where the function resides (in case you are using
  // DS_FIELD_TYPE_FUNCTION). It defaults to this plugin file. Make sure it is
  // relative to the DRUPAL_ROOT
  'file' => 'optional_file',
  // Only if you are using DS_FIELD_TYPE_FUNCTION and it defaults the
  // construct below.
  'function' => 'my_module_example_field_ds_render',
  // An array of properties or callback function that returns an array of
  // properites in the same format. Must be located in this plugin file or the
  // .module file.
  'properties' => array(
    'formatters' => array(
      'formatter_1' => t('Formatter 1'),
      'formatter_2' => t('Formatter 2'),
    ),
    'settings' => array(),
    'default' => array(
      'link' => 0,
    ),
  ),
  // If you are using properties and have the settings key, this is where you
  // specify the function that returns the value of the settings summary.
  // Function name defaults to the construct below.
  'format summary' => 'my_module_example_field_format_summary',
  // If you are using properties and have the settings key, this is where you
  // specify the function that returns the value of the settings summary.
  // Function name defaults to the construct below.
  'settings form' => 'my_module_example_field_settings_form',
);

/**
 * Renders the DS field. The default naming construct is
 * module_name_field_name_ds_render() if not otherwise specified in the
 * 'function' key of the plugin.
 *
 * @param $field
 * @return array
 */
function my_module_example_field_ds_render($field) {
  $output = array(
    '#type' => 'markup',
    '#markup' => t('This is my DS field')
  );

  return drupal_render($output);
}

/**
 * Callback for the format summary.
 *
 * @see hook_ds_field_format_summary().
 *
 * @param $field
 * @return null|string
 */
function my_module_example_field_format_summary($field) {
  return t('The format summary');
}

/**
 * Callback for the settings form.
 *
 * @see hook_ds_field_settings_form().
 *
 * @param $field
 * @return array
 */
function my_module_example_field_settings_form($field) {
  // Once the form is saved, the settings are added to the formatter_settings key.
  $settings = isset($field['formatter_settings']) ? $field['formatter_settings'] : $field['properties']['default'];

  $form = array();
  $form['link'] = array(
    '#type' => 'select',
    '#title' => t('Link'),
    '#options' => array('No', 'Yes'),
    '#default_value' => $settings['link'],
  );

  return $form;
}