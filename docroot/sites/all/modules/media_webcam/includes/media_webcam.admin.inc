<?php

/**
 * @file
 * Administrative functions for Media: Webcam.
 */

/**
 * The administrative settings form for Media: Webcam.
 */
function media_webcam_settings() {
  $form = array();

  $form['webcam'] = array(
    '#type' => 'fieldset',
    '#title' => t('Webcam flash recorder settings'),
    '#description' => t('These settings affect the webcam flash recorder.'),
    '#collapsible' => TRUE,
  );
  $form['webcam'][media_webcam_variable_name('width')] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#description' => t('Enter the width (in pixels) for the recorded video to be uploaded.'),
    '#default_value' => media_webcam_variable_get('width'),
  );
  $form['webcam'][media_webcam_variable_name('height')] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#description' => t('Enter the height (in pixels) for the recorded video to be uploaded.'),
    '#default_value' => media_webcam_variable_get('height'),
  );
  $form['webcam'][media_webcam_variable_name('no_flash')] = array(
    '#type' => 'textarea',
    '#title' => t('No flash message'),
    '#description' => t('Enter the message to be displayed in the browser in place of the webcam video recorder for users without flash.'),
    '#default_value' => media_webcam_variable_get('no_flash'),
  );

  $form['file_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Webcam file settings'),
    '#description' => t('These settings determine where to fetch and store uploaded files.'),
    '#collapsible' => TRUE,
  );
  $form['file_settings'][media_webcam_variable_name('connect_url')] = array(
    '#type' => 'textfield',
    '#title' => t('Connect URL'),
    '#description' => t('Enter the RTMP URL for the Red5 or Flash Media application. This must be in the form of %rtmp.', array('%rtmp' => 'rtmp://example.com/applicationName')),
    '#default_value' => media_webcam_variable_get('connect_url'),
  );
  $form['file_settings'][media_webcam_variable_name('base_filename')] = array(
    '#type' => 'textfield',
    '#title' => t('Base filename'),
    '#description' => t('Enter the base filename for uploaded videos, without the .flv extension.'),
    '#default_value' => media_webcam_variable_get('base_filename'),
  );
  $form['file_settings'][media_webcam_variable_name('upload_folder')] = array(
    '#type' => 'textfield',
    '#title' => t('Upload folder'),
    '#description' => t('Enter the folder where the RTMP streams will be saved for the Red5 or Flash Media application. Note that you may need to change the permissions of that folder to allow the files to be moved to the Drupal %files directory.', array('%files' => 'files')),
    '#default_value' => media_webcam_variable_get('upload_folder'),
  );
  $form['file_settings'][media_webcam_variable_name('file_directory')] = array(
    '#type' => 'textfield',
    '#title' => t('File directory'),
    '#description' => t('Enter the folder in the %files directory to store the final uploaded video.', array('%files' => 'files')),
    '#default_value' => media_webcam_variable_get('file_directory'),
  );
  $form['file_settings'][media_webcam_variable_name('max_duration')] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum duration'),
    '#description' => t('Enter the maximum duration allowed for uploaded videos, in seconds.'),
    '#default_value' => media_webcam_variable_get('max_duration'),
  );

  return system_settings_form($form);
}
