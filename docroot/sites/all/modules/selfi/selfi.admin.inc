<?php

/**
 * @file
 * Administrative page callbacks for the selfi module.
 */

/**
 * Configuration form for selfie.
 */
function selfi_webrtc_settings() {
  $form['selfi_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Selfi configuration'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['selfi_settings']['selfi_path'] = array(
    '#type' => 'textfield',
    '#title' => t('System path'),
    '#default_value' => variable_get('selfi_path', 'selfi_clicks'),
    '#description' => t('The path where uploaded files will be stored. This directory must exist and be writable by Drupal.'),
  );
  // Create a directory to store selfi.
  $selfi_path = 'public://' . variable_get('selfi_path', 'selfi_clicks');
  if (!file_prepare_directory($selfi_path, FILE_CREATE_DIRECTORY)) {
    form_set_error('selfi_path', t('The directory %directory does not exist or is not writable.', array('%directory' => $selfi_path)));
  }
  $form['selfi_settings']['selfi_preview_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Image preview width'),
    '#default_value' => variable_get('selfi_preview_width', 300),
    '#description' => t('The width of the image when selfi is taken.'),
  );
  $form['selfi_settings']['selfi_preview_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Image preview height'),
    '#default_value' => variable_get('selfi_preview_height', 200),
    '#description' => t('The height of the image when selfi is taken.'),
  );
  return system_settings_form($form);
}
