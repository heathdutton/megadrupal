<?php

/**
 * @file
 * Drush integration for addtohomescreen.
 */

/**
 * The Add to homescreen library URI.
 */
define('ADDTOHOMESCREEN_DOWNLOAD_URI', 'https://github.com/cubiq/add-to-homescreen/archive/master.zip');
define('ADDTOHOMESCREEN_DOWNLOAD_PREFIX', 'add-to-homescreen-');

/**
 * Implements hook_drush_command().
 */
function addtohomescreen_drush_command() {
  $items = array();

  // The key in the $items array is the name of the command.
  $items['addtohomescreen'] = array(
    'callback' => 'drush_addtohomescreen',
    'description' => dt('Download and install the Add to homescreen library.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'path' => dt('Optional. A path where to install the Add to homescreen library. If omitted Drush will use the default location.'),
    ),
    'aliases' => array('addtohomescreen-library'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function addtohomescreen_drush_help($section) {
  switch ($section) {
    case 'drushaddtohomescreen':
      return dt('Download and install the Add to homescreen library from https://github.com/cubiq/add-to-homescreen, default location is sites/all/libraries.');
  }
}

/**
 * Command to download the Add to homescreen library.
 */
function drush_addtohomescreen() {
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
  if ($filepath = drush_download_file(ADDTOHOMESCREEN_DOWNLOAD_URI)) {
    $filename = basename($filepath);
    $dirname = ADDTOHOMESCREEN_DOWNLOAD_PREFIX . basename($filepath, '.zip');

    // Remove any existing addtohomescreen plugin directory.
    if (is_dir($dirname) || is_dir('addtohomescreen')) {
      drush_delete_dir($dirname, TRUE);
      drush_delete_dir('addtohomescreen', TRUE);
      drush_log(dt('A existing addtohomescreen plugin was deleted from @path', array('@path' => $path)), 'notice');
    }

    // Decompress the zip archive.
    drush_tarball_extract($filename);

    // Change the directory name to "addtohomescreen" if needed.
    if ($dirname != 'addtohomescreen') {
      drush_move_dir($dirname, 'addtohomescreen', TRUE);
      $dirname = 'addtohomescreen';
    }
  }

  if (is_dir($dirname)) {
    drush_log(dt('addtohomescreen plugin has been installed in @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to install the addtohomescreen plugin to @path', array('@path' => $path)), 'error');
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);
}
