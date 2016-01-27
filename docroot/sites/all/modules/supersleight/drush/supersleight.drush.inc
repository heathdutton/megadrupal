<?php

/**
 * @file
 *   drush integration for SuperSleight.
 */

/**
 * The SuperSleight file URIs.
 */
define('SUPERSLEIGHT_DOWNLOAD_ORIG', 'http://24ways.org/code/supersleight-transparent-png-in-ie6/supersleight.zip');
define('SUPERSLEIGHT_DOWNLOAD_JQUERY', 'http://allinthehead.com/code/sleight/supersleight.plugin.js');

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
function supersleight_drush_command() {
  $items = array();

  $items['supersleight-jquery'] = array(
    'callback' => 'drush_supersleight_plugin',
    'callback arguments' => array('src' => SUPERSLEIGHT_DOWNLOAD_JQUERY),
    'description' => dt("Downloads the SuperSleight plugin."),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap.
    'arguments' => array(
      'path' => dt('Optional. A path where to install the SuperSleight plugin. If omitted Drush will use the default location.'),
    ),
    'aliases' => array('supersleightjquery', 'ss-jquery'),
  );
  $items['supersleight-plugin'] = array(
    'callback' => 'drush_supersleight_plugin',
    'callback arguments' => array('src' => SUPERSLEIGHT_DOWNLOAD_ORIG),
    'description' => dt("Downloads the SuperSleight plugin."),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap.
    'arguments' => array(
      'path' => dt('Optional. A path where to install the SuperSleight plugin. If omitted Drush will use the default location.'),
    ),
    'aliases' => array('supersleightplugin', 'ss-plugin'),
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
function supersleight_drush_help($section) {
  switch ($section) {
    case 'drush:supersleight-plugin':
      return dt("Downloads the supersleight plugin. Default location is sites/all/libraries/supersleight.");
  }
}

/**
 * Command to download the supersleight plugin.
 */
function drush_supersleight_plugin($src = NULL, $path = NULL) {
  if ($src == NULL) {
    $src = SUPERSLEIGHT_DOWNLOAD_JQUERY;
  }
  $filename = 'jquery.supersleight.js';
  $dirname = 'supersleight';
  
  if (!isset($path)) {
    $path = 'sites/all/libraries/';
  }

  // Create the path if it does not exist.
  if (!is_dir($path)) {
    drush_op('mkdir', $path);
    drush_log(dt('Directory @path was created', array('@path' => $path)), 'notice');
  }

  // Set the directory to the download location.
  $olddir = getcwd();
  chdir($path);


  // Remove any existing supersleight plugin directory
  if (is_dir($dirname)) {
    drush_log(dt('An existing SuperSleight plugin was overwritten at @path', array('@path' => $path)), 'notice');
  }
  else {
    drush_op('mkdir', $dirname);
  }

  chdir($dirname);

  if (stripos($src, '.zip')) {
    $filename = basename($src);
    if (is_file($filename)) {
      drush_op('unlink', $filename);
    }
    if (!drush_shell_exec('wget ' . $src)) {
      drush_shell_exec('curl -O ' . $src);
    }
    if (is_file($filename)) {
      // Decompress the zip archive
      drush_shell_exec('unzip -qq -o ' . $filename);
      // Remove the zip archive
      drush_op('unlink', $filename);
    }
  }
  elseif (stripos($src, '.js')) {
    if (is_file($filename)) {
      drush_op('unlink', $filename);
    }
    // Download the JS file.
    if (!drush_shell_exec('wget ' . $src . ' -O ' . $filename)) {
      drush_shell_exec('curl ' . $src . ' -o "' . $filename . '"');
    }
  }

   // Set working directory back to the previous working directory.
  chdir($olddir);

  if (is_dir($path)) {
    drush_log(dt('SuperSleight plugin has been downloaded to @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to download the supersleight plugin to @path', array('@path' => $path)), 'error');
  }
}
