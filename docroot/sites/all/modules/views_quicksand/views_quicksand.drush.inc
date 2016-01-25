<?php

/**
 * @file
 *   drush integration for quicksand.
 */

/**
 * The Quicksand plugin URI.
 */
define('QUICKSAND_DOWNLOAD_URI', 'https://github.com/razorjack/quicksand/raw/master/jquery.quicksand.js');

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
function views_quicksand_drush_command() {
  $items = array();

  // the key in the $items array is the name of the command.
  $items['quicksand-plugin'] = array(
    'callback' => 'drush_quicksand_plugin',
    'description' => dt("Downloads the Quicksand plugin."),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap.
    'arguments' => array(
      'path' => dt('Optional. A path where to install the Quicksand plugin. If omitted Drush will use the default location.'),
    ),
    'aliases' => array('quicksandplugin'),
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
function views_quicksand_drush_help($section) {
  switch ($section) {
    case 'drush:quicksand-plugin':
      return dt("Downloads the Quicksand plugin from github.com, default location is sites/all/libraries.");
  }
}

/**
 * Implements drush_MODULE_post_pm_enable().
 */
// function drush_colorbox_post_pm_enable() {
//   $modules = func_get_args();
//   if (in_array('colorbox', $modules)) {
//     drush_colorbox_plugin();
//   }
// }

/**
 * Command to download the Quicksand plugin.
 */
function drush_quicksand_plugin() {
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

  $filename = basename(QUICKSAND_DOWNLOAD_URI);
  $dirname = basename(QUICKSAND_DOWNLOAD_URI, '.js');

  // Remove any existing Quicksand plugin directory
  if (is_dir($dirname)) {
    drush_log(dt('An existing Quicksand plugin was overwritten at @path', array('@path' => $path)), 'notice');
  }
  else {
    drush_op('mkdir', $dirname);
  }
  chdir($dirname);
  // Remove any existing Quicksand plugin file
  if (is_file($filename)) {
    drush_op('unlink', $filename);
  }

  // Download the js file
  if (!drush_shell_exec('wget ' . QUICKSAND_DOWNLOAD_URI)) {
    drush_shell_exec('curl -O ' . QUICKSAND_DOWNLOAD_URI);
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);

  if (is_dir($path . '/' . $dirname)) {
    drush_log(dt('Quicksand plugin has been downloaded to @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to download the Quicksand plugin to @path', array('@path' => $path)), 'error');
  }
}
