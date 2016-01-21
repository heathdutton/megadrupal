<?php

/**
 * @file
 *   drush integration for Bootbox.js.
 */

/**
 * The Bootbox.js library URI.
 */
define('BOOTBOXJS_DOWNLOAD_URI', 'https://github.com/makeusabrew/bootbox/archive/v4.4.0.zip');

/**
 * Implementation of hook_drush_command().
 *
 * In this hook, you specify which commands your
 * drush module makes available, what it does and
 * description.
 *
 * Notice how this structure closely resembles how
 * you define menu hooks.
 *
 * See `drush topic docs-commands` for a list of recognized keys.
 *
 * @return
 *   An associative array describing your command(s).
 */
function bootboxjs_drush_command() {
  $items = array();

  // the key in the $items array is the name of the command.
  $items['bootboxjs-plugin'] = array(
    'callback' => 'drush_bootboxjs_plugin',
    'description' => dt('Download and install the Bootbox.js library.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap.
    'arguments' => array(
      'path' => dt('Optional. A path where to install the Bootbox.js library. If omitted Drush will use the default location.'),
    )
  );

  return $items;
}

/**
 * Implementation of hook_drush_help().
 *
 * This function is called whenever a drush user calls
 * 'drush help <name-of-your-command>'
 *
 * @param
 *   A string with the help section (prepend with 'drush:')
 *
 * @return
 *   A string with the help text for your command.
 */
function bootboxjs_drush_help($section) {
  switch ($section) {
    case 'drush:bootboxjs-plugin':
      return dt('Download and install the Bootbox.js library from http://bootboxjs.com, default location is sites/all/libraries.');
  }
}

/**
 * Command to download the Bootbox.js library.
 */
function drush_bootboxjs_plugin() {
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

  $filename = basename(BOOTBOXJS_DOWNLOAD_URI);
  $dirname = 'bootbox-4.4.0';

  // Remove any existing Bootbox.js library directory
  if (is_dir($dirname) || is_dir('bootboxjs')) {
    drush_delete_dir($dirname, TRUE);
    drush_delete_dir('bootboxjs', TRUE);
    drush_log(dt('A existing Bootbox.js library was deleted from @path', array('@path' => $path)), 'notice');
  }

  // Remove any existing Bootbox.js plugin zip archive
  if (is_file($filename)) {
    drush_delete_dir($filename, TRUE);
  }

  // Download the file
  drush_download_file(BOOTBOXJS_DOWNLOAD_URI);

  if (is_file($filename)) {
    // Decompress the zip archive
    drush_tarball_extract($filename);

    // Change the directory name to "bootboxjs" if needed.
    if (is_dir('bootbox-4.4.0')) {
      drush_move_dir('bootbox-4.4.0', 'bootboxjs', TRUE);
      $dirname = 'bootboxjs';
    }
  }

  if (is_dir($dirname)) {
    drush_log(dt('Bootbox.js library has been installed in @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to install the Bootbox.js library to @path', array('@path' => $path)), 'error');
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);
}
