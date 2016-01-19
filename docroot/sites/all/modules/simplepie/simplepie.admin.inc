<?php

/**
 * @file
 * Admin page callback and other admin page related functions.
 */

/**
 * Form builder; Configure the SimplePie settings.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function simplepie_admin_settings() {
  $cache_location =  variable_get('file_default_scheme', 'public') . '://' . variable_get('simplepie_cache_location', 'cache/simplepie');
  if (!file_prepare_directory($cache_location, FILE_CREATE_DIRECTORY)) {
    form_set_error('simplepie_cache_location', t('The directory %directory does not exist or is not writable.', array('%directory' => $cache_location)));
    watchdog('file system', 'The directory %directory does not exist or is not writable.', array('%directory' => $cache_location), WATCHDOG_ERROR);
  }
  $form['simplepie_cache_location'] = array(
    '#type' => 'textfield',
    '#title' => t('SimplePie Core cache location'),
    '#default_value' => variable_get('simplepie_cache_location', 'cache/simplepie'),
    '#description' => t('Subdirectory in the directory %dir where cache files will be stored.', array('%dir' => file_directory_path() . '/')),
  );

  return system_settings_form($form);
}
