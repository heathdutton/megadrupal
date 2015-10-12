<?php
/**
 * @file
 * Odir administration interface
 */

/**
 * Returns a form for changing the base directory paths
 *
 * @ingroup forms
 */
function odir_admin_settings() {
  $options = node_type_get_names();
  $form['odir_system_base'] = array(
    '#type' => 'textfield',
    '#title' => t('Base file-system directory'),
    '#default_value' => odir_system_path(),
    '#description' => t('Directory where files are being stored.'),
  );
  $form['odir_img_filter_regex'] = array(
    '#type' => 'textfield',
    '#title' => t('Regex for filtering image files'),
    '#default_value' => variable_get('odir_img_filter_regex', ODIR_IMG_FILTER_REGEX),
    '#description' => t("Don't modify this unless you know exactly what you are doing! Please note that actually only jpeg images are supported by odir_image module!"),
  );
  $form['odir_ignore_filter_regex'] = array(
    '#type' => 'textfield',
    '#title' => t('Regex for filtering files which will be ignored in directory and file listings'),
    '#default_value' => variable_get('odir_ignore_filter_regex', ODIR_IGNORE_FILTER_REGEX),
    '#description' => t("Don't modify this unless you know exactly what you are doing!"),
  );
  return system_settings_form($form);
}

/**
 * Form validation for odir_admin_settings.
 */
function odir_admin_settings_validate($form, &$form_state) {
  if ($form_state['values']['odir_system_base'] == '') {
    form_set_error('odir_system_base', t('Directory path may not be empty.'));
  }
  elseif (!file_prepare_directory($form_state['values']['odir_system_base'], FILE_CREATE_DIRECTORY)) {
    form_set_error('odir_system_base', t('Directory path must exist and must be writable.'));
  }
}
