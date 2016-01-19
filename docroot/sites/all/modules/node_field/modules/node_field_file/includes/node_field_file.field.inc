<?php

/**
 * Widget for file field.
 */
function node_field_file_widget($node_field) {
  $extensions = empty($node_field['settings']['file_extensions']) ? 'txt' :
    $node_field['settings']['file_extensions'];
  $form = array(
    '#type' => 'managed_file',
    '#title' => (!empty($node_field['title'])) ? $node_field['title'] : t('File'),
    '#default_value' => $node_field['value'],
    '#upload_location' => 'public://',
    '#upload_validators' => array(
      'file_validate_extensions' => array($extensions),
    ),
//    '#process' => array('node_field_file_save_field_value'),
  );

  return $form;
}

/**
 * Settings form for file field.
 */
function node_field_file_settings($node_field) {
  $form = array();
  $path = drupal_get_path('module', 'node_field_file');
  module_load_include('inc', 'node_field_file', 'includes/node_field_file.formatter');

  $formatters = node_field_file_formatter_info();
  $options = array();
  foreach ($formatters as $name => $item) {
    $options[$name] = $item['#title'];
  }
  // Display formatters.
  $default = empty($node_field['settings']['formatter']) ? key($formatters) : $node_field['settings']['formatter'];
  $form['formatter'] = array(
    '#type' => 'select',
    '#title' => t('Display'),
    '#options' => $options,
    '#default_value' => $default,
  );
  $form['link_target'] = array(
    '#type' => 'select',
    '#title' => t('Link target'),
    '#options' => array(
      '_blank' => t('Open in new window'),
      '_self' => t('Open in the same window'),
    ),
    '#default_value' => empty($node_field['settings']['link_target']) ? '_blank' : $node_field['settings']['link_target'],
    '#states' => array(
      'visible' => array('#edit-settings-formatter' => array('value' => 'node_field_file_link')),
    ),
  );
  $styles = image_styles();
  $options = array('original' => t('original'));
  foreach ($styles as $key => $style) {
    $options[$key] = $style['name'];
  }
  // File extensions.
  $extensions = empty($node_field['settings']['file_extensions']) ? array('txt') :
    $node_field['settings']['file_extensions'];
  $form['file_extensions'] = array(
    '#type' => 'textfield',
    '#title' => t('Allowed file extensions'),
    '#element_validate' => array('_file_generic_settings_extensions'),
    '#default_value' => $extensions,
    '#description' => t('Separate extensions with a space or comma and do not include the leading dot.'),
    '#required' => TRUE,
  );
  // Image style.
  $default = empty($node_field['settings']['image_style']) ? key($options) :
    $node_field['settings']['image_style'];
  $form['image_style'] = array(
    '#type' => 'select',
    '#title' => t('Image styles'),
    '#options' => $options,
    '#default_value' => $default,
    '#attached' => array(
      'js' => array($path . '/themes/js/node_field_file.settings.js'),
    ),
  );
  // Size.
  $default = empty($node_field['settings']['image_style']) ? key($options) :
    $node_field['settings']['image_style'];
  $form['size'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
  );
  $default = empty($node_field['settings']['size']['width']) ?
    NODE_FIELD_FILE_DEFAULT_WIDTH : $node_field['settings']['size']['width'];
  $form['size']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => $default,
  );
  $default = empty($node_field['settings']['size']['height']) ?
    NODE_FIELD_FILE_DEFAULT_HEIGHT : $node_field['settings']['size']['height'];
  $form['size']['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#default_value' => $default,
  );
  // File system.
  $default = empty($node_field['settings']['file_system']) ? 'public' :
    $node_field['settings']['file_system'];
  $form['file_system'] = array(
    '#type' => 'radios',
    '#title' => t('Upload destination'),
    '#options' => array(
      'public' => t('Public files'),
      'private' => t('Private files'),
    ),
    '#description' => t('Select where the final files should be stored.
      Private file storage has significantly more overhead than public files,
      but allows restricted access to files within this field.'),
    '#default_value' => $default,
  );

  return $form;
}

/**
 * Formatter for file field.
 */
function node_field_file_formatter($node_field) {
  $output = NULL;
  module_load_include('inc', 'node_field_file', 'includes/node_field_file.formatter');

  $formatters = node_field_file_formatter_info();
  if (isset($node_field['settings']['formatter'])) {
    $formatter = $node_field['settings']['formatter'];
  }
  else {
    $formatter = key($formatters);
  }
  if (isset($formatters[$formatter])) {
    $output = call_user_func($formatters[$formatter]['#formatter'], $node_field);
  }

  return $output;
}