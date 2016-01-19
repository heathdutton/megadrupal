<?php
/**
 * @file
 * Administration function for epub_export_mobi module.
 */

/**
 * Default settings form to set the path to the kindlegen binary.
 */
function epub_export_settings() {
  $form['epub_export_run_as_user'] = array(
    '#type' => 'textfield',
    '#title' => t('Build EPUB as'),
    '#description' => t('If your site requires special permissions during the bundling process create a user with the appropriate level of access and then choose that user here. Otherwise the anonymous user is used.'),
    '#size' => 30,
    '#maxlength' => 60,
    '#autocomplete_path' => 'user/autocomplete',
    '#default_value' => variable_get('epub_export_run_as_user', ''),
    '#weight' => -1,
  );
  return system_settings_form($form);
}

