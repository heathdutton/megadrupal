<?php

/**
 * @file
 *   Drush integration for PhotoSwipe.
 */

/**
 * The PhotoSwipe plugin URI.
 */
define('PHOTOSWIPE_DOWNLOAD_URL', 'https://github.com/dimsemenov/PhotoSwipe/archive/v4.0.5.zip');
define('PHOTOSWIPE_DOWNLOAD_PREFIX', 'PhotoSwipe-');

/**
 * Implements hook_drush_command().
 */
function photoswipe_drush_command() {
  $items = array();

  // the key in the $items array is the name of the command.
  $items['photoswipe-plugin'] = array(
    'callback' => 'drush_photoswipe_plugin',
    'description' => dt('Download and install the PhotoSwipe plugin.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap.
    'arguments' => array(
      'path' => dt('Optional. A path where to install the PhotoSwipe plugin. If omitted Drush will use the default location.'),
    ),
    'aliases' => array('photoswipeplugin'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function photoswipe_drush_help($section) {
  switch ($section) {
    case 'drush:photoswipe-plugin':
      return dt('Download and install the PhotoSwipe plugin from GitHub, default location is sites/all/libraries.');
  }
}

/**
 * Implements drush_MODULE_post_pm_enable().
 */
function drush_photoswipe_post_pm_enable() {
  $modules = func_get_args();
  if (in_array('photoswipe', $modules)) {
    drush_photoswipe_plugin();
  }
}

/**
 * Command to download the PhotoSwipe plugin.
 */
function drush_photoswipe_plugin() {
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

  // Download the zip archive
  if ($filepath = drush_download_file(PHOTOSWIPE_DOWNLOAD_URL)) {
    $filename = basename($filepath);
    $dirname = PHOTOSWIPE_DOWNLOAD_PREFIX . ltrim(basename($filepath, '.zip'), 'v');

    // Remove any existing PhotoSwipe plugin directory
    if (is_dir($dirname) || is_dir('photoswipe')) {
      drush_delete_dir($dirname, TRUE);
      drush_delete_dir('photoswipe', TRUE);
      drush_log(dt('A existing PhotoSwipe plugin was deleted from @path', array('@path' => $path)), 'notice');
    }

    // Decompress the zip archive
    drush_tarball_extract($filename);

    // Change the directory name to "photoswipe" if needed.
    if ($dirname != 'photoswipe') {
      drush_move_dir($dirname, 'photoswipe', TRUE);
      $dirname = 'photoswipe';
    }
  }

  if (is_dir($dirname)) {
    drush_log(dt('PhotoSwipe plugin has been installed in @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to install the PhotoSwipe plugin to @path', array('@path' => $path)), 'error');
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);
}
