<?php

/**
 * @file
 * Ocrad functions.
 */

/**
 * Implements hook_form_settings().
 */
function ocrad_form_settings($form, $form_state) {
  $form['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Ruta de ocrad'),
    '#default_value' => variable_get('ocrad_path'),
    '#description' => t('Ruta de ocrad. Por ejemplo: /usr/bin/ocrad.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;

}

/**
 * Submit handler for ocrad_form_settings_submit().
 */
function ocrad_form_settings_submit($form, &$form_state) {

    variable_set('ocrad_path', $form["path"]["#value"]);
    if (file_exists(variable_get('ocrad_path'))) {
        drupal_set_message(t('The form has been submitted. The path is: !path', array('!path' => $form["path"]["#value"])));
    } else if (!file_exists(variable_get('ocrad_path'))) {
        drupal_set_message(t('The binary is not in the path: !path', array('!path' => $form["path"]["#value"])), 'error');
    } 

}

/**
 * Submit handler for ocrad_form_upload().
 */
function ocrad_form_upload($form_state) {

  $form['file'] = array(
    '#type' => 'file',
    '#title' => t('Image File'),
    '#description' => t('Upload an image file: .pbm, .pgm or .ppm'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;

}

/**
 * Validate handler for ocrad_form_upload_validate().
 */
function ocrad_form_upload_validate($form, &$form_state) {

  $file = file_save_upload('file', array('file_validate_extensions' => array('pgm pbm ppm')));
  // If the file passed validation:
  if ($file) {
    // Move the file, into the Drupal file system.
    if ($file = file_move($file, 'public://')) {
      // Save the file for use in the submit handler.
      $form_state['storage']['file'] = $file;
    }
    else {
      form_set_error('file', t("Failed to write the uploaded file to the site's file folder."));
    }
  }
  else {
    form_set_error('file', t('No file was uploaded.'));
  }
}

/**
 * Submit handler for ocrad_form_upload_submit().
 */
function ocrad_form_upload_submit($form, &$form_state) {
  $file = $form_state['storage']['file'];
  // We are done with the file, remove it from storage.
  unset($form_state['storage']['file']);
  // Make the storage of the file permanent.
  $file->status = FILE_STATUS_PERMANENT;
  // Save file status.
  file_save($file);
  if (file_exists(variable_get('ocrad_path', '/usr/bin/ocrad')) && fileperms(variable_get('ocrad_path', '/usr/bin/ocrad'))) {
      exec(variable_get('ocrad_path', '/usr/bin/ocrad') . ' ' . drupal_realpath('public://') . '/' . $file->filename, $out);
      // Set a response to the user.
      drupal_set_message(t('The form has been submitted and @filename has been saved and the text
is: @result',
   array(
    '@filename' => $file->filename,
    '@result' => $out[0])));
  }
}