<?php

/**
 * @file
 * drush integration for Shariff.
 */

/**
 * The Shariff library URI.
 */
define('SHARIFF_LIBRARY_DOWNLOAD_URI', 'https://github.com/heiseonline/shariff/archive/master.zip');

/**
 * Implements hook_drush_command().
 *
 * In this hook, you specify which commands your
 * drush module makes available, what it does and description.
 *
 * Notice how this structure closely resembles how you define menu hooks.
 *
 * See 'drush topic docs-commands' for a list of recognized keys.
 */
function shariff_drush_command() {
  $items = array();

  // The key in the $items array is the name of the command.
  $items['shariff-library'] = array(
    'callback' => 'download_shariff_library',
    'description' => dt('Download and install the shariff library.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'path' => dt('Optional. A path where to install the Shariff library. If omitted Drush will use the default location.'),
    ),
    'aliases' => array('shariff'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 *
 * This function is called whenever a drush user calls
 * 'drush help <name-of-your-command>'.
 *
 * @section string.
 *   A string with the help section (prepend with 'drush:').
 */
function shariff_drush_help($section) {
  switch ($section) {
    case 'drush:shariff-library':
      return dt('Download and install the shariff library from https://github.com/heiseonline/shariff/, default location is sites/all/libraries.');
  }
}

/**
 * Command to download the Shariff library.
 */
function download_shariff_library() {
  $args = func_get_args();
  if (!empty($args[0])) {
    $path = $args[0];
  }
  else {
    $path = 'sites/all/libraries';
  }

  // Create the path if it does not exist.
  if (!is_dir($path)) {
    drush_op('mkdir', $path);
    drush_log(dt('Directory @path was created', array('@path' => $path)), 'notice');
  }

  // Set the directory to the download location.
  $olddir = getcwd();
  chdir($path);

  // Download the zip archive.
  if ($filepath = drush_download_file(SHARIFF_LIBRARY_DOWNLOAD_URI)) {
    $filename = basename($filepath);
    $dirname = basename($filepath, '.zip');

    // Remove any existing Shariff library directory.
    if (is_dir($dirname) || is_dir('shariff')) {
      drush_delete_dir($dirname, TRUE);
      drush_delete_dir('shariff', TRUE);
      drush_log(dt('Existing Shariff library was deleted from @path', array('@path' => $path)), 'notice');
    }

    // Decompress the zip archive.
    drush_tarball_extract($filename, $dirname);
    drush_log($dirname);
    // Change the directory name to "shariff" if needed.
    if (is_dir('master')) {
      drush_move_dir('master/shariff-master', 'shariff', TRUE);
      // Removes leftover master dir.
      drush_delete_dir('master', TRUE);
    }

    $dirname = 'shariff';
  }

  if (is_dir($dirname)) {
    drush_log(dt('The Shariff library has been installed in @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to install the Shariff library to @path', array('@path' => $path)), 'error');
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);
}
