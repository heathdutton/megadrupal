<?php
/**
 * @file
 * The actual functionality for the Drupal Reset module.
 *
 * Provides the functions which delete the files and drop the database tables.
 */

/**
 * Delete all public and private files.
 */
function _drupal_reset_delete_files() {
  _drupal_reset_rrmdir(drupal_realpath('public://'));
  _drupal_reset_rrmdir(drupal_realpath('private://'));
}

/**
 * Delete all database tables.
 *
 * @global array $databases
 */
function _drupal_reset_drop_database() {
  global $databases;

  $tables = db_find_tables($databases['default']['default']['prefix'] . '%');

  foreach ($tables as $table) {
    db_drop_table($table);
  }
}

/**
 * Recursive directory deletion.
 */
function _drupal_reset_rrmdir($dir) {
  if (!empty($dir) && is_dir($dir)) {
    $objects = scandir($dir);
    foreach ($objects as $object) {
      if ($object !== '.' && $object !== '..') {
        $this_object = $dir . '/' . $object;
        if (filetype($this_object) === 'dir') {
          _drupal_reset_rrmdir($this_object);
        }
        else {
          unlink($this_object);
        }
      }
    }
    rmdir($dir);
  }
}

/**
 * Check if the installation uses a single-database and a simple prefix.
 */
function _drupal_reset_is_supported_database_configuration() {
  global $databases;
  if (isset($databases['default']['default'])
    && count($databases['default']) === 1
    && isset($databases['default']['default']['prefix'])
    && is_string($databases['default']['default']['prefix'])) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}
