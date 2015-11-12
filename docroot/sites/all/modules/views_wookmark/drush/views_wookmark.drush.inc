<?php

/**
 * @file
 *   drush integration for wookmark.
 */

/**
 * The wookmark plugin URI.
 */
define('WOOKMARK_DOWNLOAD_URI', 'https://github.com/GBKS/Wookmark-jQuery/archive/master.zip');

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
function views_wookmark_drush_command() {
  $items = array();

  // The key in the $items array is the name of the command.
  $items['wookmark-plugin'] = array(
    'callback' => 'drush_wookmark_plugin',
    'description' => dt('Download and install the wookmark plugin.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap.
    'arguments' => array(
      'path' => dt('Optional. A path where to install the wookmark plugin. If omitted Drush will use the default location.'),
    ),
    'aliases' => array('wookmark'),
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
function views_wookmark_drush_help($section) {
  switch ($section) {
    case 'drush:wookmark-plugin':
      return dt('Download and install the wookmark plugin, default location is sites/all/libraries.');
  }
}

/**
 * Command to download the wookmark plugin.
 */
function drush_wookmark_plugin() {
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
  if ($filepath = drush_download_file(WOOKMARK_DOWNLOAD_URI)) {
    $filename = basename($filepath);
    $dirname =  'Wookmark-jQuery-master';

    // Remove any existing wookmark plugin directory.
    if (is_dir($dirname) || is_dir('wookmark')) {
      drush_delete_dir($dirname, TRUE);
      drush_delete_dir('wookmark', TRUE);
      drush_log(dt('A existing wookmark plugin was deleted from @path', array('@path' => $path)), 'notice');
    }

    // Decompress the zip archive.
    drush_tarball_extract($filename);

    // Change the directory name to "wookmark" if needed.
    if ($dirname != 'wookmark') {
      drush_move_dir($dirname, 'wookmark', TRUE);
      $dirname = 'wookmark';
    }
  }

  if (is_dir($dirname)) {
    drush_log(dt('wookmark plugin has been installed in @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to install the wookmark plugin to @path', array('@path' => $path)), 'error');
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);
}