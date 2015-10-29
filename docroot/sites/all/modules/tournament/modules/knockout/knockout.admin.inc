<?php

/**
 * @file
 * Admin page callback file for the knockout tournament sub                                  module.
 */

/**
 * Form builder; Configure knockout tournament settings for this site.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function knockout_tournament_admin_settings() {
  $form = array();

  $form['knockout_background_image_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to custom background image'),
    '#description' => t('The path to the file you would like to use as your image.'),
    '#default_value' => variable_get('knockout_background_image_path',""),
  );
  $form['knockout_background_image_upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload background image'),
    '#maxlength' => 40,
    '#description' => t("If you don't have direct file access to the server, use this field to upload your image.")
  );

  $form['#submit'][] = 'knockout_tournament_admin_settings_submit';
  $form['#validate'][] ='knockout_tournament_admin_settings_validate';

  return system_settings_form($form);
}

/**
 * Submit handler for the knockout_tournament_admin_settings() form.
 */
function knockout_tournament_admin_settings_submit($form, &$form_state) {
  $values = $form_state['values'];

  // If the user uploaded a new background image, save it to a permanent location
  // and set the file path.
  if ($file = $values['knockout_background_image_upload']) {
    unset($values['knockout_background_image_upload']);
    $filename = file_unmanaged_copy($file->uri);
    $form_state['values']['knockout_background_image_path'] = $filename;
  }
}

/**
 * Validator for the knockout_tournament_admin_settings() form.
 */
function knockout_tournament_admin_settings_validate($form, &$form_state) {
  // Handle file uploads.
  $validators = array('file_validate_is_image' => array());

  // Check for a new uploaded background image.
  $file = file_save_upload('knockout_background_image_upload', $validators);
  if (isset($file)) {
    // File upload was attempted.
    if ($file) {
      // Put the temporary file in form_values so we can save it on submit.
      $form_state['values']['knockout_background_image_upload'] = $file;
    }
    else {
      // File upload failed.
      form_set_error('knockout_background_image_upload', t('The background image could not be uploaded.'));
    }
  }

  // If the user provided a path for a background image, make sure a file
  // exists at that path.
  if ($form_state['values']['knockout_background_image_path']) {
    require_once 'modules/system/system.admin.inc';
    $path = _system_theme_settings_validate_path($form_state['values']['knockout_background_image_path']);
    if (!$path) {
      form_set_error('knockout_background_image_path', t('The custom background image path is invalid.'));
    }
  }
}
