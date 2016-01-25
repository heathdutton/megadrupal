<?php

/**
 * Widget for file field.
 */
function sky_field_file_widget($sky_field) {
  $extensions = empty($sky_field['settings']['file_extensions']) ? 'txt' :
    $sky_field['settings']['file_extensions'];
  $form = array(
    '#type' => 'managed_file',
    '#title' => t('File'),
    '#default_value' => $sky_field['value'],
    '#upload_validators' => array(
      'file_validate_extensions' => array($extensions),
    ),
  );

  return $form;
}

/**
 * Settings form for file field.
 */
function sky_field_file_settings($sky_field) {
  $form = array();
  $path = drupal_get_path('module', 'sky_field_file');
  module_load_include('inc', 'sky_field_file', 'includes/sky_field_file.formatter');

  $formatters = sky_field_file_formatter_info();
  $options = array();
  foreach ($formatters as $name => $item) {
    $options[$name] = $item['#title'];
  }
  // Display formatters.
  $default = empty($sky_field['settings']['formatter']) ? key($formatters) :
    $sky_field['settings']['formatter'];
  $form['formatter'] = array(
    '#type' => 'select',
    '#title' => t('Display'),
    '#options' => $options,
    '#default_value' => $default,
  );
  $styles = image_styles();
  $options = array('original' => t('original'));
  foreach ($styles as $key => $style) {
    $options[$key] = $style['name'];
  }
  // File extensions.
  $extensions = empty($sky_field['settings']['file_extensions']) ? array('txt') :
    $sky_field['settings']['file_extensions'];
  $form['file_extensions'] = array(
    '#type' => 'textfield',
    '#title' => t('Allowed file extensions'),
    '#element_validate' => array('_file_generic_settings_extensions'),
    '#default_value' => $extensions,
    '#description' => t('Separate extensions with a space or comma and do not include the leading dot.'),
    '#required' => TRUE,
  );
  // Image style.
  $default = empty($sky_field['settings']['image_style']) ? key($options) :
    $sky_field['settings']['image_style'];
  $form['image_style'] = array(
    '#type' => 'select',
    '#title' => t('Image styles'),
    '#options' => $options,
    '#default_value' => $default,
    '#attached' => array(
      'js' => array($path . '/themes/js/sky_field_file.settings.js'),
    ),
  );
  // Size.
  $default = empty($sky_field['settings']['image_style']) ? key($options) :
    $sky_field['settings']['image_style'];
  $form['size'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
  );
  $default = empty($sky_field['settings']['size']['width']) ?
    SKY_FIELD_FILE_DEFAULT_WIDTH : $sky_field['settings']['size']['width'];
  $form['size']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => $default,
  );
  $default = empty($sky_field['settings']['size']['height']) ?
    SKY_FIELD_FILEEFAULT_HEIGHT : $sky_field['settings']['size']['height'];
  $form['size']['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#default_value' => $default,
  );
  // File system.
  $default = empty($sky_field['settings']['file_system']) ? 'public' :
    $sky_field['settings']['file_system'];
  foreach (file_get_stream_wrappers(STREAM_WRAPPERS_WRITE_VISIBLE) as $scheme => $info) {
    $system[$scheme] = check_plain($info['description']);
  }

  $form['file_system'] = array(
    '#type' => 'radios',
    '#title' => t('Upload destination'),
    '#options' => $system,
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
function sky_field_file_formatter($sky_field) {
  $output = NULL;
  module_load_include('inc', 'sky_field_file', 'includes/sky_field_file.formatter');

  $formatters = sky_field_file_formatter_info();
  if (isset($sky_field['settings']['formatter'])) {
    $formatter = $sky_field['settings']['formatter'];
  }
  else {
    $formatter = key($formatters);
  }
  if (isset($formatters[$formatter])) {
    $output = call_user_func($formatters[$formatter]['#formatter'], $sky_field);
  }

  return $output;
}