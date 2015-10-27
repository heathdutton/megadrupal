<?php

/**
 * @file
 * Drush integration for ref debug.
 */

/**
 * The PHP-ref library URI.
 */
define('PHP_REF_LIBRARY_DOWNLOAD_URI', 'https://github.com/digitalnature/php-ref/archive/master.zip');

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
function ref_debug_drush_command() {
  $items = array();

  // The key in the $items array is the name of the command.
  $items['php-ref-library'] = array(
    'callback' => 'download_php_ref_library',
    'description' => dt('Download and install the php-ref libray.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'path' => dt('Optional. A path where to install the Ref debug plugin. If omitted Drush will use the default location.'),
    ),
    'aliases' => array('phpref'),
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
function ref_debug_drush_help($section) {
  switch ($section) {
    case 'drush:php-ref-library':
      return dt('Download and install the php ref library from https://github.com/digitalnature/php-ref, default location is sites/all/libraries.');
  }
}

/**
 * Command to download the ref debug plugin.
 */
function download_php_ref_library() {
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
  if ($filepath = drush_download_file(PHP_REF_LIBRARY_DOWNLOAD_URI)) {
    $filename = basename($filepath);
    $dirname = basename($filepath, '.zip');

    // Remove any existing Ref debug plugin directory.
    if (is_dir($dirname) || is_dir('php-ref')) {
      drush_delete_dir($dirname, TRUE);
      drush_delete_dir('php-ref', TRUE);
      drush_log(dt('A existing Ref debug plugin was deleted from @path', array('@path' => $path)), 'notice');
    }

    // Decompress the zip archive.
    drush_tarball_extract($filename, $dirname);
    drush_log($dirname);
    // Change the directory name to "php-ref" if needed.
    if (is_dir('master')) {
      drush_move_dir('master/php-ref-master', 'php-ref', TRUE);
      // Removes leftover master dir.
      drush_delete_dir('master', TRUE);
      // Removes unwanted files.
      drush_delete_dir('php-ref/tests', TRUE);
      drush_delete_dir('php-ref/composer.json', TRUE);
    }

    $dirname = 'php-ref';
  }

  if (is_dir($dirname)) {
    drush_log(dt('The php-ref library has been installed in @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to install the php ref library to @path', array('@path' => $path)), 'error');
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);
}
