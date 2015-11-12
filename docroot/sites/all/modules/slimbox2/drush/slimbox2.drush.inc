<?php

/**
 * @file
 * Drush integration: Slimbox2.
 *
 * Slimbox2 is a module/wrapper that integrates
 * Slimbox2 jQuery plugin from www.digitalia.be
 * into Drupal websites.
 *
 * author: Andrew Wasson <https://drupal.org/user/127091>
 */

/**
 * The slimbox2 plugin URI.
 */
define('SLIMBOX2_DOWNLOAD_URI', 'https://slimbox.googlecode.com/files/slimbox-2.05.zip');

/**
 * Implements hook_drush_command().
 */
function slimbox2_drush_command() {
  $items = array();

  $items['slimbox2-plugin'] = array(
    'callback' => 'drush_slimbox2_plugin',
    'description' => dt('Download and install the slimbox2 plugin.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'path' => dt('Optional. A path where to install the slimbox2 plugin. If omitted Drush will use the default location.'),
    ),
    'aliases' => array('slimbox2plugin'),
  );

  return $items;
}

/**
 * Callback to download the slimbox2 plugin and unzip it at its destination.
 */
function drush_slimbox2_plugin() {
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
  $drushdir = getcwd();
  chdir($path);

  // Download the zip archive.
  if ($filepath = drush_download_file(SLIMBOX2_DOWNLOAD_URI)) {
    $filename = basename($filepath);
    $dirname = "slimbox2";

    // Remove any existing slimbox2 plugin directory.
    if (is_dir($dirname) || is_dir('slimbox2')) {
      drush_delete_dir($dirname, TRUE);
      drush_delete_dir('slimbox2', TRUE);
      drush_log(dt('A existing slimbox2 plugin was deleted from @path', array('@path' => $path)), 'notice');
    }

    // Decompress the zip archive into $dirname.
    drush_tarball_extract($filename, $dirname);

    // Change the directory name to "slimbox2" if needed.
    if ($dirname != 'slimbox2') {
      drush_move_dir($dirname, 'slimbox2', TRUE);
      $dirname = 'slimbox2';
    }
  }

  if (is_dir($dirname)) {
    drush_log(dt('slimbox2 plugin has been installed in @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to install the slimbox2 plugin to @path', array('@path' => $path)), 'error');
  }

  // Set working directory back to the previous working directory.
  chdir($drushdir);
}

/**
 * Implements hook_drush_help().
 */
function slimbox2_drush_help($section) {
  switch ($section) {
    case 'drush:slimbox2-plugin':
      return dt('Download and install the slimbox2 plugin from code.google.com/p/slimbox, default location is sites/all/libraries.');
  }
}
