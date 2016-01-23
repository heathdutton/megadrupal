<?php

/**
 * @file
 * Implementation of the administration settings form for the module.
 *
 */

/**
 * The dynamic background administration settings form.
 *
 */
function dynamic_background_admin_settings() {
  $form = array(
    '#tree' => TRUE,
  );

  // Load default values
  $default = variable_get('dynamic_background_setting', array());

  $form['dynamic_background_setting']['num_of_pictures'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of images'),
    '#description' => t('Enter the number of images that are possible to upload.'),
    '#size' => 8,
    '#required' => TRUE,
    '#default_value' => isset($default['num_of_pictures']) ? $default['num_of_pictures'] : '',
  );

  $form['dynamic_background_setting']['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Upload path'),
    '#description' => t('The path inside the files folder to upload the images to.'),
    '#size' => 25,
    '#required' => TRUE,
    '#default_value' => isset($default['path']) ? $default['path'] : '',
  );

  $form['dynamic_background_setting']['extensions'] = array(
    '#type' => 'textfield',
    '#title' => t('Allowed file types'),
    '#description' => t('Allowed files extension to upload. The list should be seperated by spaces.'),
    '#size' => 30,
    '#required' => TRUE,
    '#default_value' => isset($default['extensions']) ? $default['extensions'] : 'jpg jpeg png',
  );

  // Add image style to the form.
  $form += dynamic_background_image_style_form('dynamic_background_image_style');

  // Add css behaviour form to the form.
  $form += dynamic_background_css_behaviour_form('dynamic_background_css');

  // Set drupal system settings form and add validation function.
  $form = system_settings_form($form);
  $form['#validate'][] = 'dynamic_background_admin_settings_validate';

  return $form;
}

/**
 * Validation function for the administration settings form.
 */
function dynamic_background_admin_settings_validate($form, &$form_state) {
  // Validate number of images
  if (!is_numeric($form_state['values']['dynamic_background_setting']['num_of_pictures'])) {
    form_set_error('num_of_pictures', t('Please enter a number.'));
  }

  // Create upload path
  $path = file_default_scheme() . '://' . check_plain($form_state['values']['dynamic_background_setting']['path']);
  if (!file_prepare_directory($path, FILE_CREATE_DIRECTORY)) {
    form_set_error('path', t('The entered path could not be created.'));
  }

  /**
   * @todo Check that extension is only numbers and letters
   */
}