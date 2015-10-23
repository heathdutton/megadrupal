<?php

/**
 *
 * @file
 * Functions for drupal admin.
 */

// $Id$

include_once('file_maintenance.core.inc');

define('URI_QUERY_PARAMETER', 'path');

/**
 * Default values
 */
define('FILE_MAINTENANCE_FILES_DISPLAYED', 'all');
define('FILE_MAINTENANCE_SHOW_TEMP_DIRS', TRUE);
define('FILE_MAINTENANCE_TEMP_DIRS', "public://styles/\npublic://languages/");
define('FILE_MAINTENANCE_RECURSIVE', TRUE);
// TODO
define('FILE_MAINTENANCE_FILE_LIST_COLUMN_', TRUE);

/**
 *
 *
 * @param $form
 * @param $form_state
 */
function file_maintenance_settings_form($form, &$form_state) {
  $form['file_maintenance_temp_dirs'] = array(
    '#type' => 'textarea',
    '#title' => t('Directories in the file system that are used for caching (e.g. for image styles).'),
    '#default_value' => variable_get('file_maintenance_temp_dirs', FILE_MAINTENANCE_TEMP_DIRS),
    '#size' => 60,
    '#description' => t(
      'Enter the paths that contain temporary files.'
      . ' <strong>One path per line</strong>'),
  );
  return system_settings_form($form);
}

/**
 * Modified version of core's (defective) file_usage_list() (file.inc)
 *
 * @param stdClass $file
 */
function file_maintenance_file_usage_list(stdClass $file) {
  $result = db_select('file_usage', 'f')
    ->fields('f', array('module', 'type', 'id', 'count'))
    ->condition('fid', $file->fid)
    ->condition('count', 0, '>')
    ->execute();
  $references = array();
  foreach ($result as $usage) {
    $references[$usage->module][$usage->type][$usage->id] = $usage->count;
  }
  return $references;
}
