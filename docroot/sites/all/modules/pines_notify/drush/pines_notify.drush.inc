<?php

/**
 * @file
 * Drush integration for Pines Notify.
 */

/**
 * The Pines Notify plugin URI.
 */
define('PNOTIFY_DOWNLOAD_URI', 'https://github.com/sciactive/pnotify/archive/master.zip');
define('PNOTIFY_DOWNLOAD_PREFIX', 'pnotify-');

/**
 * Implements hook_drush_command().
 */
function pines_notify_drush_command() {
  $items = array();

  // The key in the $items array is the name of the command.
  $items['pnotify-plugin'] = array(
    'callback' => 'drush_pines_notify_plugin',
    'description' => dt('Download and install the Pines Notify plugin.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'path' => dt('Optional. A path where to install the Pines Notify plugin. If omitted Drush will use the default location.'),
    ),
    'aliases' => array('pnotifyplugin'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function pines_notify_drush_help($section) {
  switch ($section) {
    case 'drush:pnotify-plugin':
      return dt('Download and install the Pines Notify plugin from pinesframework.org/pnotify, default location is sites/all/libraries.');
  }
}

/**
 * Command to download the Pines Notify plugin.
 */
function drush_pines_notify_plugin() {
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
  $old_dir = getcwd();
  chdir($path);

  // Download the zip archive.
  if ($filepath = drush_download_file(PNOTIFY_DOWNLOAD_URI)) {
    $filename = basename($filepath);
    $dir_name = PNOTIFY_DOWNLOAD_PREFIX . basename($filepath, '.zip');

    // Remove the existing 'pnotify' directory if it exists.
    if (is_dir($dir_name) || is_dir('pnotify')) {
      drush_delete_dir($dir_name, TRUE);
      drush_delete_dir('pnotify', TRUE);
      drush_log(dt('A existing Pines Notify plugin was deleted from @path', array('@path' => $path)), 'notice');
    }

    // Decompress the zip archive.
    drush_tarball_extract($filename);

    // Change the directory name to 'pnotify' if needed.
    if ($dir_name != 'pnotify') {
      drush_move_dir($dir_name, 'pnotify', TRUE);
      $dir_name = 'pnotify';
    }
  }

  if (is_dir($dir_name)) {
    drush_log(dt('Pines Notify plugin has been installed in @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to install the Pines Notify plugin to @path', array('@path' => $path)), 'error');
  }

  // Set working directory back to the previous working directory.
  chdir($old_dir);
}
