<?php

/**
 * @author "Ahmad Hesamzadeh" <mngafa@gmail.com>
 * @file Administration pages and forms
 */

/**
 * Form builder for 'admin/config/media/nginx-securelink/handlers/byfile'
 */
function ngsl_access_byfile_admin_form($form, &$form_state) {
  $form['ngsl_access_byfile_filesystem_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Filesystm path'),
    '#description' => t('Where to look for config files? (Please enter an absolute path which is valid for your OS.) Please don\'t add a trailing slash. Use slashes as directory separators, even for windows OS.'),
    '#default_value' => variable_get('ngsl_access_byfile_filesystem_path', ''),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}

/**
 * Form validator
 */
function ngsl_access_byfile_admin_form_validate($form, &$form_state) {
  // validate filesstem path
  if (!is_dir($form_state['values']['ngsl_access_byfile_filesystem_path'])) {
    form_set_error('ngsl_access_byfile_filesystem_path', 'Filesystm path is not a directory. Please enter a valid directory path.');
  }
}
