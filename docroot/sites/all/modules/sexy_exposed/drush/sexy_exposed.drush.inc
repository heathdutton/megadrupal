<?php

/**
 * @file
 *   Drush integration for Sexy Exposed.
 */

define('SEXY_EXPOSED_DDCL_URI', 'http://dropdown-check-list.googlecode.com/files/dropdown-check-list.1.3.zip');

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
 *
 * @return
 *   An associative array describing your command(s).
 */
function sexy_exposed_drush_command() {
  $items = array();

  $items['se-plugin'] = array(
    'callback' => 'drush_se_plugin',
    'description' => dt("Downloads the dropdown-check-list plugin (used by Sexy Exposed) to 'sites/all/libraries'."),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap.
    'aliases' => array('se_plugin'),
  );

  return $items;
}

/**
 * Download the ddcl plugin.
 * Big up to colorbox module!
 */
function drush_se_plugin() {
  if (!drush_shell_exec('type unzip')) {
    return drush_set_error(dt('Missing dependency: unzip. Install it before using this command.'));
  }

  $path = 'sites/all/libraries/ddcl';

  // Create the path if it does not exist.
  if (!is_dir($path)) {
    drush_op('mkdir', $path);
    drush_log(dt('Directory @path was created', array('@path' => $path)), 'notice');
  }

  // Set the directory to the download location.
  $olddir = getcwd();
  chdir($path);

  $filename = basename(SEXY_EXPOSED_DDCL_URI);

  // Overwrite any existing ddcl plugin directory.
  if (is_dir('js')) {
    drush_log(dt('A existing ddcl plugin was overwritten at @path', array('@path' => $path)), 'notice');
  }

  // Remove any existing ddcl plugin zip archive.
  if (is_file($filename)) {
    drush_op('unlink', $filename);
  }

  // Download the zip archive.
  if (!drush_shell_exec('wget ' . SEXY_EXPOSED_DDCL_URI)) {
    drush_shell_exec('curl -O ' . SEXY_EXPOSED_DDCL_URI);
  }

  if (is_file($filename)) {
    // Decompress the zip archive.
    drush_shell_exec('unzip -qq -o ' . $filename);
    // Remove the zip archive.
    drush_op('unlink', $filename);
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);

  if (is_dir($path . '/js')) {
    drush_log(dt('Dropdown-check-list plugin has been downloaded to @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to download the Dropdown-check-list plugin to @path', array('@path' => $path)), 'error');
  }
}

