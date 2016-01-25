<?php

/**
 * @file
 * Theme settings file for the Gob cl theme.
 */

require_once dirname(__FILE__) . '/template.php';

/**
 * Implements hook_form_FORM_alter().
 */
function gob_cl_form_system_theme_settings_alter(&$form, &$form_state) {
  // You can use this hook to append your own theme settings to the theme
  // settings form for your subtheme. You should also take a look at the
  // 'extensions' concept in the Omega base theme.
  $form['gob_cl_image'] = array(
    '#type' => 'fieldset',
    '#title'         => t('Header Image Settings'),
    '#description'   => t("Upload header image"),
    '#collapsible' => TRUE, 
    '#collapsed' => TRUE, 
  );
  
  $form['gob_cl_image']['gob_cl_header_image_default'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Use default header image'),
    '#description'   => T('Check here if you want the theme to use the header image supplied with it.'),
    '#default_value' => theme_get_setting('gob_cl_header_image_default', $GLOBALS['theme_key']),
  );

  $form['gob_cl_image']['settings'] = array(
    '#type' => 'container',
    '#states' => array(
      // Hide the header image settings when using the default image.
      'invisible' => array(
        'input[name="gob_cl_header_image_default"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['gob_cl_image']['settings']['gob_cl_header_image_path'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Path to header image'),
    '#description'   => t("The path to the file you would like to use as your header image file instead of the default image."),
    '#default_value' => theme_get_setting('gob_cl_header_image_path', $GLOBALS['theme_key']),
  );

  $element = &$form['gob_cl_image']['settings']['gob_cl_header_image_path'];

  // If path is a public:// URI, display the path relative to the files
  // directory; stream wrappers are not end-user friendly.
  $original_path = $element['#default_value'];
  $friendly_path = NULL;
  if (file_uri_scheme($original_path) == 'public') {
    $friendly_path = file_uri_target($original_path);
    $element['#default_value'] = $friendly_path;
  }

  $form['gob_cl_image']['settings']['gob_cl_header_image'] = array(
    '#type'          => 'file',
    '#title'         => t('Upload header image'),
    '#description'   => t("Select a header image."),
  );
  

  array_unshift($form['#submit'], 'gob_cl_theme_settings_form_submit');
  $form['#validate'][] = 'gob_cl_theme_settings_form_validate';
}

/**
 * Validator for the gob_cl_form_system_theme_settings() form.
 */
function gob_cl_theme_settings_form_validate($form, &$form_state) {
  
  // Handle file uploads.
  $validators = array('file_validate_is_image' => array());

  // Check for a new uploaded image.
  $file = file_save_upload('gob_cl_header_image', $validators);
  if (isset($file)) {
    // File upload was attempted.
    if ($file) {
      // Put the temporary file in form_values so we can save it on submit.
      $form_state['values']['gob_cl_header_image'] = $file;
    }
    else {
      // File upload failed.
      form_set_error('gob_cl_header_image', t('The header image could not be uploaded.'));
    }
  }

  // If the user provided a path for a image file, make sure a file
  // exists at that path.
  if (!empty($form_state['values']['gob_cl_header_image_path'])) {
    $path = _system_theme_settings_validate_path($form_state['values']['gob_cl_header_image_path']);
    if (!$path) {
      form_set_error('gob_cl_header_image_path', t('The custom image path is invalid.'));
    }
  }

}

/**
 * Form submit handler for the theme settings form.
 */
function gob_cl_theme_settings_form_submit($form, &$form_state) {

  $values = &$form_state['values'];

  // If the user uploaded a new header image, save it to a permanent location
  // and use it in place of the default theme-provided file.
  if (!empty($values['gob_cl_header_image'])) {
    $file = $values['gob_cl_header_image'];
    unset($values['gob_cl_header_image']);
    $file_upload = file_unmanaged_copy($file->uri);
    $values['gob_cl_header_image_default'] = 0;
    $values['gob_cl_header_image_path'] = $file_upload;
  }

  if (!empty($values['gob_cl_header_image_path'])) {
    $values['gob_cl_header_image_path'] = _system_theme_settings_validate_path($values['gob_cl_header_image_path']);
  }

}