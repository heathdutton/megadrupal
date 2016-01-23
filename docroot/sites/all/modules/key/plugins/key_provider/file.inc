<?php

/**
 * @file
 * Plugin definition for the File key provider.
 */

$plugin = array(
  'title' => t('File'),
  'description' => t('Use a file, preferably one that is outside the web root directory.'),
  'key get value' => 'key_provider_file_get_key',
  'provider settings form' => 'key_provider_file_settings_form',
  'config form validate' => 'key_provider_file_config_form_validate',
  'key value form' => 'key_provider_file_key_value_form',
  'key value obscure' => 'key_provider_file_key_value_obscure',
);

/**
 * Callback function to return a key from a file.
 */
function key_provider_file_get_key($config) {
  if (empty($config['provider_settings']['file_location'])) {
    return NULL;
  }
  $file = $config['provider_settings']['file_location'];

  // Make sure the file exists and is readable.
  if (!is_file($file) || !is_readable($file)) {
    return NULL;
  }

  $key_value = file_get_contents($file);

  // Base64-decode if necessary.
  if (isset($config['provider_settings']['base64_encoded']) && $config['provider_settings']['base64_encoded']) {
    $key_value = base64_decode($key_value);
  }

  return $key_value;
}

/**
 * Settings form for the File key provider.
 */
function key_provider_file_settings_form($defaults) {
  $form = array();

  $form['file_location'] = array(
    '#type' => 'textfield',
    '#title' => t('File location'),
    '#description' => t('The location of the file in which the key will be stored. The path may be absolute (e.g., %abs), relative to the Drupal directory (e.g., %rel), or defined using a stream wrapper (e.g., %str).', array(
      '%abs' => '/etc/keys/foobar.key',
      '%rel' => '../keys/foobar.key',
      '%str' => 'private://keys/foobar.key',
      )
    ),
    '#default_value' => isset($defaults['file_location']) ? $defaults['file_location'] : '',
    '#required' => TRUE,
  );

  return $form;
}

/**
 * Validate callback for the configuration form.
 */
function key_provider_file_config_form_validate($form, &$form_state) {
  $file = $form_state['values']['provider_settings']['file_location'];

  // Verify that the file exists.
  if (!is_file($file)) {
    form_set_error('file_location', t('A file at the specified location does not exist.'));
  }
  // Verify that the file is readable.
  elseif (!is_readable($file)) {
    form_set_error('file_location', t('The file at the specified location is not readable.'));
  }
}

/**
 * Obscure the key on configuration form.
 */
function key_provider_file_key_value_obscure($key_value, $config) {
  return '';
}
