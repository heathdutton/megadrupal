<?php
/**
 * @file
 * Packaging administration UI implementation.
 */

/**
 * Packaging settings form.
 */
function ph_package_admin_settings($form, &$form_state) {
  $form['directory'] = array(
    '#type' => 'fieldset',
    '#title' => t('Directory'),
  );
  $form['directory']['ph_package_working_directory'] = array(
    '#type' => 'textfield',
    '#title' => t('Packaging directory'),
    '#default_value' => ph_package_working_directory(),
    '#required' => TRUE,
    '#element_validate' => array('ph_repository_validate_directory'),
    '#description' => t('Specify a writable directory to package repositories to.'),
  );
  $form['directory']['ph_package_archive_directory'] = array(
    '#type' => 'textfield',
    '#title' => t('Archive directory'),
    '#default_value' => variable_get('ph_package_archive_directory', 'public://'),
    '#required' => TRUE,
    '#element_validate' => array('ph_package_validate_local_directory', 'ph_repository_validate_directory'),
    '#description' => t('Specify a writable directory to archive repositories to. This directory must be inside the Drupal file system, as prefixed with one of the Drupal path schemes, e.g. "public://" (see help below).'),
  );
  $form['directory']['ph_package_archive_replace'] = array(
    '#type' => 'radios',
    '#title' => t('Existing archive file replace behavior'),
    '#default_value' => variable_get('ph_package_archive_replace', FILE_EXISTS_REPLACE),
    '#options' => array(
      FILE_EXISTS_RENAME => t('Rename'),
      FILE_EXISTS_REPLACE => t('Replace existing'),
      FILE_EXISTS_ERROR => t('Abort'),
    ),
    '#description' => t('Specify what to do when creating an archive with the same file name as an existing file.'),
  );
  $form['directory']['help'] = array(
    '#markup' => theme('ph_repository_directory_help'),
    '#weight' => 5,
  );

  $form['stamp'] = array(
    '#type' => 'fieldset',
    '#title' => t('Stamp'),
    '#description' => t('Configure settings for package stamping (i.e. annotating project manifest information).'),
  );

  $options = array();
  foreach (ph_package_get_stamp_info() as $name => $stamp_info) {
    $options[$name] = $stamp_info['label'];
  }
  $form['stamp']['ph_package_stamp'] = array(
    '#type' => 'select',
    '#title' => t('Stamp handler'),
    '#default_value' => variable_get('ph_package_stamp', 'default'),
    '#options' => $options,
    '#required' => TRUE,
    '#description' => t('The handler used to write additional details to info files.'),
  );
  $form['stamp']['ph_package_stamp_header'] = array(
    '#type' => 'textarea',
    '#title' => t('Stamp header'),
    '#default_value' => variable_get('ph_package_stamp_header', ''),
    '#description' => t('Specify header text to insert before metadata in the info files. This text is automatically commented when inserted. You may use token replacement patterns.'),
  );

  // Add token help.
  if (module_exists('token')) {
    $form['token_help'] = array(
      '#markup' => theme('token_tree'),
    );
  }

  return system_settings_form($form);
}
