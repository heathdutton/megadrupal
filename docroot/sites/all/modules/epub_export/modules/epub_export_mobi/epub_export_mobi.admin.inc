<?php
/**
 * @file
 * Administration function for epub_export_mobi module.
 */

/**
 * Default settings form to set the path to the kindlegen binary.
 */
function epub_export_mobi_settings() {
  $form['epub_export_mobi_binary_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to kindlegen binary'),
    '#description' => t('The full path, including the kindlegen binary name.'),
    '#default_value' => variable_get('epub_export_mobi_binary_path', '/opt/local/bin/kindlegen'),
  );
  return system_settings_form($form);
}

/**
 * Form validation function to ensure the executable exists and is executable.
 */
function epub_export_mobi_settings_validate($form, &$form_state) {
  $binary = $form_state['values']['epub_export_mobi_binary_path'];
  if (!file_exists($binary)) {
    form_set_error('epub_export_mobi_binary_path', t('The file "%file" does not exist.', array('%file' => $binary)));
  }
  else {
    if (!is_executable($binary)) {
      form_set_error('epub_export_mobi_binary_path', t('The file "%file" is not executable.', array('%file' => $binary)));
    }
  }
}

