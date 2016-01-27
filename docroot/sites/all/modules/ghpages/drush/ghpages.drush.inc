<?php

/**
 * @file
 * drush integration for Git.php
 */

/**
 * The Git.php library URI.
 */
define('GHPAGES_GITPHP_DOWNLOAD_URI', 'https://github.com/kbjr/Git.php/archive/master.zip');

/**
 * Implements hook_drush_command().
 */
function ghpages_drush_command() {
  $items = array();

  // The key in the $items array is the name of the command.
  $items['download-gitphp'] = array(
    'callback' => 'drush_ghpages',
    'description' => dt('Download and install the Git.php library.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'path' => dt('Optional. A path where to install the Git.php library. If omitted Drush will use the default location.'),
    ),
    'aliases' => array('gitphp'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function ghpages_drush_help($section) {
  switch ($section) {
    case 'drush:download-gitphp':
      return dt('Download and install the Git.php library from github.com/kbjr/Git.php, default location is sites/all/libraries.');
  }
}

/**
 * Implements drush_MODULE_post_pm_enable().
 */

/**
 * Command to download the Git.php library.
 */
function drush_ghpages() {
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
  if ($filepath = drush_download_file(GHPAGES_GITPHP_DOWNLOAD_URI)) {
    $filename = 'master.zip';
    $dirname = 'Git.php-master';

    // Remove any existing Colorbox plugin directory.
    if (is_dir($dirname) || is_dir('Git.php')) {
      drush_delete_dir($dirname, TRUE);
      drush_delete_dir('Git.php', TRUE);
      drush_log(dt('A existing Git.php library was deleted from @path', array('@path' => $path)), 'notice');
    }

    // Decompress the zip archive.
    drush_tarball_extract($filename);

    // Change the directory name to "colorbox" if needed.
    if ($dirname != 'Git.php') {
      drush_move_dir($dirname, 'Git.php', TRUE);
      $dirname = 'Git.php';
    }
  }

  if (is_dir($dirname)) {
    drush_log(dt('Git.php library has been installed to @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to install Git.php to @path', array('@path' => $path)), 'error');
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);
}
