<?php

/**
 * @file
 *   drush integration for kraken.
 */

/**
 * The kraken-php library URI.
 */
define('KRAKEN_DOWNLOAD_URI', 'https://github.com/kraken-io/kraken-php/archive/master.zip');

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
function kraken_drush_command() {
  $items = array();

  // The key in the $items array is the name of the command.
  $items['kraken-library'] = array(
    'callback' => 'drush_kraken_library',
    'description' => dt('Download and install the kraken-php library.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap.
    'arguments' => array(
      'path' => dt('Optional. A path where to install the kraken-php library. If
        omitted Drush will use the default location.'),
    ),
    'aliases' => array('kraken'),
  );

  return $items;
}

/**
 * Implementation of hook_drush_help().
 *
 * This function is called whenever a drush user calls
 * 'drush help <name-of-your-command>'.
 *
 * @param
 *   A string with the help section (prepend with 'drush:').
 *
 * @return
 *   A string with the help text for your command.
 */
function kraken_drush_help($section) {
  switch ($section) {
    case 'drush:kraken-library':
      return dt('Download and install the kraken library. The default location
        is sites/all/libraries.');
  }
}

/**
 * Command to download the kraken library.
 */
function drush_kraken_library() {
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
  if ($filepath = drush_download_file(KRAKEN_DOWNLOAD_URI)) {
    $filename = basename($filepath);
    $dirname =  'kraken-php-master';

    // Remove any existing kraken library directory.
    if (is_dir($dirname) || is_dir('kraken-php')) {
      drush_delete_dir($dirname, TRUE);
      drush_delete_dir('kraken-php', TRUE);
      drush_log(dt('An existing kraken library was deleted from @path', array('@path' => $path)), 'notice');
    }

    // Decompress the zip archive.
    drush_tarball_extract($filename);

    // Change the directory name to "kraken-php" if needed.
    if ($dirname != 'kraken-php') {
      drush_move_dir($dirname, 'kraken-php', TRUE);
      $dirname = 'kraken-php';
    }
  }

  if (is_dir($dirname)) {
    drush_log(dt('kraken library has been installed in @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to install the kraken library to @path', array('@path' => $path)), 'error');
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);
}