<?php

/**
 * @file
 * Drush integration for viewerjs.
 */

/**
 * The ViewerJS plugin URI.
 */
define('VIEWERJS_DOWNLOAD_URI', 'https://github.com/Strae/Viewer.js/archive/master.zip');

/**
 * Implements hook_drush_command().
 *
 * In this hook, you specify which commands your
 * drush module makes available, what it does and
 * description.
 *
 * Notice how this structure closely resembles how
 * you define menu hooks.
 *
 * See `drush topic docs-commands` for a list of recognized keys.
 */
function viewerjs_drush_command() {
  $items = array();

  // The key in the $items array is the name of the command.
  $items['viewerjs-plugin'] = array(
    'callback' => 'drush_viewerjs_plugin',
    'description' => dt('Download and install the viewerjs plugin.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'path' => dt('Optional. A path where to install the viewerjs plugin. If omitted Drush will use the default location.'),
    ),
    'aliases' => array('viewerjsplugin'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 *
 * This function is called whenever a drush user calls
 * 'drush help <name-of-your-command>'.
 */
function viewerjs_drush_help($section) {
  switch ($section) {
    case 'drush:viewerjs-plugin':
      return dt('Download and install the viewerjs plugin from https://github.com/Strae/Viewer.js, default location is sites/all/libraries.');
  }
}

/**
 * Command to download the viewerjs plugin.
 */
function drush_viewerjs_plugin() {
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
  if ($filepath = drush_download_file(VIEWERJS_DOWNLOAD_URI)) {
    $filename = basename($filepath);
    $dirname = basename($filepath, '.zip');

    // Remove any existing viewerjs plugin directory.
    if (is_dir($dirname) || is_dir('viewerjs')) {
      drush_delete_dir($dirname, TRUE);
      drush_delete_dir('viewerjs', TRUE);
      drush_log(dt('A existing viewerjs plugin was deleted from @path', array('@path' => $path)), 'notice');
    }

    // Decompress the zip archive.
    drush_tarball_extract($filename);
    // Change the directory name to "viewerjs" if needed.
    if ($dirname != 'viewerjs') {
      drush_move_dir('Viewer.js-master', 'viewerjs', TRUE);
      $dirname = 'viewerjs';
    }
  }

  if (is_dir($dirname)) {
    drush_log(dt('viewerjs plugin has been installed in @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to install the viewerjs plugin to @path', array('@path' => $path)), 'error');
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);
}
