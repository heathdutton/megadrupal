<?php
/**
 * @file
 * Settings interface.
 */

/**
 * Callback function for db_track_settings_form().
 */
function db_track_settings_form($form, &$form_state) {
  $form['db_track_excluded_url'] = array(
    '#type' => 'textarea',
    '#title' => t('Excluded forms by URL'),
    '#description' => t('Specify pages by using their paths. Enter one path per line. The "*" character is a wildcard. Example paths are admin/config for the configuration page and admin/config/* for every sub configuration pages. Forms on these pages will not be tracked.'),
    '#default_value' => variable_get('db_track_excluded_url', ''),
    '#rows' => 8,
  );

  return system_settings_form($form, TRUE);
}
