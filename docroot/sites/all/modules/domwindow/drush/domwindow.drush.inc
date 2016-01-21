<?php

/**
 * @file
 *   drush integration for domwindow.
 */

/**
 * The DOMWindow plugin URI.
 */
define('DOMWINDOW_DOWNLOAD_URI', 'http://swip.codylindley.com/jquery.DOMWindow.js');

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
function domwindow_drush_command() {
  $items = array();

  // the key in the $items array is the name of the command.
  $items['domwindow-plugin'] = array(
    'callback' => 'drush_domwindow_plugin',
    'description' => dt("Downloads the DOMWindow plugin."),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap.
    'arguments' => array(
      'path' => dt('Optional. A path where to install the DOMWindow plugin. If omitted Drush will use the default location.'),
    ),
    'aliases' => array('domwindowplugin'),
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
function domwindow_drush_help($section) {
  switch ($section) {
    case 'drush:domwindow-plugin':
      return dt("Downloads the DOMWindow plugin from http://swip.codylindley.com, default location is sites/all/libraries.");
  }
}

/**
 * Implements drush_MODULE_post_pm_enable().
 */
// function drush_domwindow_post_pm_enable() {
//   $modules = func_get_args();
//   if (in_array('domwindow', $modules)) {
//     drush_domwindow_plugin();
//   }
// }

/**
 * Command to download the DOMWindow plugin.
 */
function drush_domwindow_plugin() {
  $filename = basename(DOMWINDOW_DOWNLOAD_URI);
  $dirname = drupal_strtolower(basename(DOMWINDOW_DOWNLOAD_URI, '.js'));
  
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


  // Remove any existing DOMWindow plugin directory
  if (is_dir($dirname)) {
    drush_log(dt('A existing DOMWindow plugin was overwritten at @path', array('@path' => $path)), 'notice');
  }
  else {
    drush_op('mkdir', $dirname);
  }

  chdir($dirname);

  // Remove any existing DOMWindow plugin zip archive
  if (is_file($filename)) {
    drush_op('unlink', $filename);
  }

  // Download the zip archive
  if (!drush_shell_exec('wget ' . DOMWINDOW_DOWNLOAD_URI)) {
    drush_shell_exec('curl -O ' . DOMWINDOW_DOWNLOAD_URI);
  }

   // Set working directory back to the previous working directory.
  chdir($olddir);

  if (is_dir($path)) {
    drush_log(dt('DOMWindow plugin has been downloaded to @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to download the DOMWindow plugin to @path', array('@path' => $path)), 'error');
  }
}