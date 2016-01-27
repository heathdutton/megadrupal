<?php

/**
 * Implements hook_libraries_info().
 *
 * @return mixed
 */
function feature_wysiwyg_editor_libraries_info() {

  // Define a library, its not actually a real library but serves as a
  // container for all CKEditor skins.
  $libraries['ckeditor_skins'] = array(
    'name' => 'CKeditor Skins',
    'version callback' => '_ckeditor_skins',
  );

  return $libraries;
}

/**
 * Version callback for the ckeditor_skins library.
 * @return int
 */
function _ckeditor_skins() {
  // Because it isnt actaully a library there is no file to scan for a version
  // number so we return the arbitrary number 1.
  return 1;
}

/**
 * Implements hook_form_alter().
 * @param $form
 * @param $form_state
 * @param $form_id
 */
function feature_wysiwyg_editor_form_alter(&$form, &$form_state, $form_id) {

  if ($form_id == 'imce_profile_form') {

    // Load the profile
    $profile = imce_load_profile($form['pid']['#value']);

    // Add our excluded directories field.
    $form['profile']['excluded'] = array(
      '#type' => 'textarea',
      '#title' => 'Exclude directories',
      '#description' => 'Specify directories to exclude from the file browser by entering their paths. Enter one path per line.',
      '#default_value' => $profile['excluded'],
      '#weight' => 1,
    );

  }


  // Only do this for CKEditor wysiwygs
  if ($form_id == 'wysiwyg_profile_form' && $form['editor']['#value'] == 'ckeditor') {

    // Machine name of the input format
    $format = $form['format']['#value'];

    // Load the wysiwyg profile
    $profile = wysiwyg_get_profile($format);

    // Load the settings
    $settings = $profile->settings;

    // get the path to the skins folder.
    $skins_path = libraries_get_path('ckeditor_skins');

    // Return a list of potential themes
    $themes_scan = file_scan_directory($skins_path, '/.*/', array('recurse' => false), 0);

    $themes = array();

    // Add a default
    $themes['default'] = 'Default';

    // Build the options array of available themes.
    foreach ($themes_scan as $key => $value) {
      $themes[$value->name] = ucwords($value->name);
    }

    // Add a fapi element to the admin UI.
    $form['appearance']['skin'] = array(
      '#type' => 'select',
      '#options' => $themes,
      '#title' => t('Choose a skin'),
      '#description' => t('You can upload new skins to the sites/all/ckeditor_skins folder.'),
      '#default_value' => isset($settings['skin']) ? $settings['skin'] : 'default'
    );

    // Add a text area to collect a list of paths to remove from IMCE


  }
}
