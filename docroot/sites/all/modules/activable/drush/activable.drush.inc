<?php

/**
 * @file
 *   drush integration for activable.
 */

/**
 * The Activable script.
 */
define('ACTIVABLE_DOWNLOAD_URI', 'https://github.com/downloads/louisremi/Activable/activable.zip');

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
function activable_drush_command() {
  $items = array();

  // the key in the $items array is the name of the command.
  $items['activable-plugin'] = array(
    'callback' => 'drush_activable_plugin',
    'description' => dt("Downloads the Activable script."),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap.
    'arguments' => array(
      'path' => dt('Optional. A path where to install the Activable plugin. If omitted Drush will use the default location.'),
    ),
    'aliases' => array('activableplugin'),
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
function activable_drush_help($section) {
  switch ($section) {
    case 'drush:activable-plugin':
      return dt("Downloads the Activable plugin from https://github.com/louisremi/Activable#activable, default location is sites/all/libraries.");
  }
}

/**
 * Command to download the Activable plugin.
 */
function drush_activable_plugin() {
  if (!drush_shell_exec('type unzip')) {
    return drush_set_error(dt('Missing dependency: unzip. Install it before using this command.'));
  }

  $filename = basename(ACTIVABLE_DOWNLOAD_URI);
  $dirname = drupal_strtolower(basename(ACTIVABLE_DOWNLOAD_URI, '.zip'));
  
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
    drush_log(dt('Directory @path was created', array('@path' => $path)), 'success');
  }

  // Set the directory to the download location.
  $olddir = getcwd();
  chdir($path);

  // Remove any existing Activable plugin directory
  if (is_dir($dirname)) {
    drush_log(dt('An existing Activable plugin was overwritten at @path', array('@path' => $path)), 'notice');
  }
  else {
    drush_op('mkdir', $dirname);
  }

  chdir($dirname);

  // Remove any existing Activable plugin zip archive
  if (is_file($filename)) {
    drush_op('unlink', $filename);
  }

  // Download the zip archive
  if (!drush_shell_exec('wget ' . ACTIVABLE_DOWNLOAD_URI)) {
    drush_shell_exec('curl -O -L ' . ACTIVABLE_DOWNLOAD_URI);
  }
  
  if (is_file($filename)) {
    // Decompress the zip archive
    drush_shell_exec('unzip -qq -o ' . $filename);
    // Remove the zip archive
    drush_op('unlink', $filename);
  }

   // Set working directory back to the previous working directory.
  chdir($olddir);

  if (is_dir($path)) {
    drush_log(dt('Activable plugin has been downloaded to @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to download the Activable plugin to @path', array('@path' => $path)), 'error');
  }
}
