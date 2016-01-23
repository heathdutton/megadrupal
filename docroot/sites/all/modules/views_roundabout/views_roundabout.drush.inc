<?php

/**
 * @file
 *   drush integration for roundabout.
 */

/**
 * The Roundabout plugin Version.
 */
define('ROUNDABOUT_VERSION', 'v2.3');

/**
 * The Roundabout plugin URI.
 */
define('ROUNDABOUT_DOWNLOAD_URI', 'https://github.com/fredhq/roundabout/zipball/' . ROUNDABOUT_VERSION);

/**
 * The Roundabout plugin name.
 */
define('ROUNDABOUT_PLUGIN_NAME', 'jquery.roundabout');

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
function views_roundabout_drush_command() {
  $items = array();

  // the key in the $items array is the name of the command.
  $items['roundabout-plugin'] = array(
      'callback' => 'drush_roundabout_plugin',
      'description' => dt("Downloads the Roundabout plugin."),
      'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap.
      'arguments' => array(
          'path' => dt('Optional. A path where to install the Roundabout plugin. If omitted Drush will use the default location.'),
      ),
      'aliases' => array('roundaboutplugin'),
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
function views_roundabout_drush_help($section) {
  switch ($section) {
    case 'drush:roundabout-plugin':
      return dt("Downloads the Roundabout plugin " . ROUNDABOUT_VERSION . " from github.com, default location is sites/all/libraries.");
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
 * Command to download the Roundabout plugin.
 */
function drush_roundabout_plugin() {
  if (!drush_shell_exec('type unzip')) {
    return drush_set_error(dt('Missing dependency: unzip. Install it before using this command.'));
  }

  $args = func_get_args();
  if (!empty($args[0])) {
    $path = $args[0];
  } else {
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

  $filename = basename(ROUNDABOUT_DOWNLOAD_URI);
  $dirname = ROUNDABOUT_PLUGIN_NAME;

  // Remove any existing Roundabout plugin directory
  if (is_dir($dirname)) {
    drush_log(dt('An existing Roundabout plugin was overwritten at @path', array('@path' => $path)), 'notice');
    drush_shell_exec('rm -R ' . $dirname);
  }

  drush_op('mkdir', $dirname);

  chdir($dirname);
  // Remove any existing Roundabout plugin zip archive
  /* if (is_file($filename)) {
    drush_op('unlink', $filename);
    } */

  // Download the zip archive
  if (!drush_shell_exec('wget ' . ROUNDABOUT_DOWNLOAD_URI)) {
    drush_shell_exec('curl -O ' . ROUNDABOUT_DOWNLOAD_URI);
  }

  if (is_file($filename)) {
    // Decompress the zip archive
    drush_shell_exec('unzip -qq -o ' . $filename);
    // Remove the zip archive
    drush_op('unlink', $filename);
    // Move files from archive folder to jquery.roundabout
    // As the name of the folder can change scan for it's name
    foreach (scandir('./') as $item) {
      if ($item != '.' && $item != '..') {
        $dir = $item;
      }
    }
    drush_shell_exec('mv ' . $dir . '/* ./');
    drush_op('rmdir', $dir);
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);

  if (is_dir($path . '/' . $dirname)) {
    drush_log(dt('Roundabout plugin ' . ROUNDABOUT_VERSION . ' has been downloaded to @path', array('@path' => $path)), 'success');
  } else {
    drush_log(dt('Drush was unable to download the Roundabout plugin to @path', array('@path' => $path)), 'error');
  }
}
