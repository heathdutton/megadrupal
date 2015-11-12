<?php

/**
 * @file
 * Drush plugin for making files public or private.
 * Also works to move files within a schema.
 */

/**
 * Implements hook_drush_command().
 */
function privatize_drush_command() {
  $items['privatize'] = array(
    'description' => 'Convert all private files to public files and changes the default file scheme to private://.',
    'examples' => array(
      'drush privatize' => 'Makes all public files private',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'aliases' => array('private'),
  );
  $items['publicize'] = array(
    'description' => 'Convert all public files to private files and changes the default file scheme to public://.',
    'examples' => array(
      'drush publicize' => 'Makes all private files public',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'aliases' => array('socialize', 'public'),
  );
  $items['move-files'] = array(
    'description' => 'Makes all files public.',
    'arguments' => array(
      'source' => 'source directory URI, i.e., public://example.',
      'destination' => 'destination directory URI.',
    ),
    'examples' => array(
      'drush move-files public://source-example public://destination-example' => 'Move the files.',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );
  return $items;
}

/**
 * Drush privatize command callback.
 */
function drush_privatize() {
  if (drush_confirm(dt('Do you want to make all files private?'))) {
    _privatize('public', 'private');
  }
}

/**
 * Drush publicize command callback.
 */
function drush_privatize_publicize() {
  if (drush_confirm(dt('Do you want to make all files public?'))) {
    _privatize('private', 'public');
  }
}

/**
 * Drush move file command callback.
 *
 * @param string $source
 *   Source directory URI, i.e., public://example.
 * @param string $destination
 *   Destination directory URI.
 */
function drush_privatize_move_files($source, $destination) {
  if (drush_confirm(dt('Do you want to move all files in %source to %destination?', array('%source' => $source, '%destination' => $destination)))) {
    _privatize_move_files($source, $destination);
  }
}

/**
 * Make files public.
 *
 * @param string $source_schema
 *   Source schema, either 'public' or 'private'.
 * @param string $destination_schema
 *   Destination schema, either 'public' or 'private'.
 *
 * @return bool
 *   TRUE if any files need to be moved, otherwise FALSE.
 */
function _privatize($source_scheme, $destination_scheme) {
  // Make the default schema public.
  variable_set('file_default_scheme', $destination_scheme);

  // Change the schema on file fields.
  $fields = field_read_fields();
  foreach ($fields as $field) {
    if (isset($field['settings']['uri_scheme']) && $field['settings']['uri_scheme'] == $source_scheme) {
      $field['settings']['uri_scheme'] = $destination_scheme;
      field_update_field($field);
    }
  }

  // Move files from private to public.
  return _privatize_move_files("$source_scheme://", "$destination_scheme://");
}

/**
 * Move files from one path to another.
 *
 * @param string $source
 *   Source directory URI, i.e., public://example.
 * @param string $destination
 *   Destination directory URI.
 *
 * @return bool
 *   TRUE if any files need to be moved, otherwise FALSE.
 */
function _privatize_move_files($source, $destination) {
  // Find all the private files and make a plan for moving them.
  // If we can move the directory rather than the file, this is
  // faster and safer (permissions don't change), so do this.
  $query = new EntityFieldQuery;
  $results = $query
    ->entityCondition('entity_type', 'file')
    ->propertyCondition('uri', $source . '%', 'LIKE')
    ->execute();
  if (empty($results['file'])) {
    return FALSE;
  }

  $files = file_load_multiple(array_keys($results['file']));
  $moves = array();
  foreach ($files as $file) {
    $file->new_uri = str_replace($source, $destination, $file->uri);
    $dir = dirname($file->new_uri);

    // Break the directory into it's paths, so moves are stored hierarchically.
    $move = &$moves;
    $paths = explode('/', substr($dir, strlen($destination)));
    foreach ($paths as $path) {
      $move = &$move[$path];
    }

    // Make the move plan. If the directory exists, move the file, otherwise
    // attempt to move the directory first.
    if (!file_exists($dir)) {
      $move['#source'] = dirname($file->uri);
    }
    $move['#destination'] = $dir;
    $move['#files'][] = $file;
  }

  // Now move the files and update the database.
  _privatize_move_organized_files($moves);
  return TRUE;
}

/**
 * Move files using the organized plan created in _privatize_move_files().
 *
 * This recursively walks the move plan.
 *
 * @param array $move
 *   Deep array of directory hierarchy move plan.
 */
function _privatize_move_organized_files($move) {
  // Check if there is something to move at this hierarchical level.
  if (isset($move['#destination'])) {
    // Try moving the directory first.
    if (_privatize_move_organized_using_dir($move)) {
      return;
    }
    // When the directory move fails, move each file individually.
    _privatize_move_organized_using_files($move);
  }

  // Move the sub-directories.
  $children = element_children($move);
  if ($children) {
    foreach ($children as $child) {
      _privatize_move_organized_files($move[$child]);
    }
  }
}

/**
 * Try moving the entire directory.
 *
 * This is safest move option. It guarantees all files (managed and unmanaged)
 * are moved. It preserves directory and file permissions.
 *
 * @param array $move
 *   #source - source directory URI.
 *   #destination - destination directory URI.
 *   #files - array of File objects found in source directory.
 *
 * @return bool
 *   TRUE on success, FALSE on failure.
 */
function _privatize_move_organized_using_dir($move) {
  if (isset($move['#source'])) {
    // Get the real paths because rename() does not work across wrappers.
    // Use rename() because file_unmanaged_move() is not for directories.
    $source_path = drupal_realpath($move['#source']);
    $destination_path = drupal_realpath($move['#destination']);
    if (rename($source_path, $destination_path)) {
      // The directory move succeeded, update the database.
      _privatize_dbupdate_organized_files($move);

      watchdog('privatize', 'Directory moved from %source to %destination.', array('%source' => $source_path, '%destination' => $destination_path));
      return TRUE;
    }
  }
  return FALSE;
}

/**
 *
 * @param array $move
 *   #source - source directory URI.
 *   #destination - destination directory URI.
 *   #files - array of File objects found in source directory.
 *
 * @return bool
 *   TRUE if all files are moved, FALSE on any one failure.
 */
function _privatize_move_organized_using_files($move) {
  // Move the files individually.
  // Make sure the directory exists first.
  if (file_prepare_directory($move['#destination'], FILE_CREATE_DIRECTORY|FILE_MODIFY_PERMISSIONS)) {
    // Move all of the files in the directory.
    $status = TRUE;
    foreach ($move['#files'] as $file) {
      if (!file_exists($file->uri)) {
        $status = FALSE;
        watchdog('privatize', 'Missing file %source, file not moved.', array('%source' => $file->uri), WATCHDOG_ERROR);
      }
      elseif (file_move($file, $file->new_uri)) {
        watchdog('privatize', 'File moved from %source to %destination.', array('%source' => $file->uri, '%destination' => $file->new_uri));
      }
    }
  }
  else {
    $status = FALSE;
    watchdog('privatize', 'Directory %dir cannot be prepared.', array('%dir' => $move['#destination']), WATCHDOG_ERROR);
  }
  return $status;
}

/**
 * Update the database URI for files moved in the organized plan.
 *
 * This recursively walks the move plan.
 *
 * @param array $move
 *   #source - source directory URI.
 *   #destination - destination directory URI.
 *   #files - array of File objects found in source directory.
 */
function _privatize_dbupdate_organized_files(&$move) {
  // Update the files for this directory.
  foreach ($move['#files'] as $file) {
    $file->uri = $file->new_uri;
    if (file_exists($file->uri)) {
      file_save($file);
    }
    else {
      watchdog('privatize', 'Missing file %source, directory moved but file missing.', array('%source' => $file->uri), WATCHDOG_ERROR);
    }
  }

  // Also update the file paths for all sub-directories.
  $children = element_children($move);
  foreach ($children as $child) {
    _privatize_dbupdate_organized_files($move[$child]);
  }
}
