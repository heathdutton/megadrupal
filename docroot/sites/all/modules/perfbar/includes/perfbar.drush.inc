<?php
/**
 * @file
 * Provides drush commands for perfBar module.
 *
 * Courtesy of cluke009 at https://drupal.org/node/1231378 .
 */

/**
 * Define the perfBar library location.
 */
define('PERFBAR_LIBRARY_URI', 'https://github.com/lafikl/perfBar/archive/v0.1.0.zip');
/**
 * Implements hook_drush_command().
 */
function perfbar_drush_command() {
  $items['perfbar_plugin'] = array(
    'callback' => 'drush_perfbar_plugin',
    'description' => dt("Downloads the perfBar library from Github."),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'path' => dt('Path is optional. Will use default location if omitted. Default location is sites/all/libraries.'),
    ),
    'aliases' => array('perfdl'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function perfbar_drush_help($section) {
  switch ($section) {
    case 'drush:perfbar-plugin':
      return dt("Downloads the perfBar library from Github. Downloads to sites/all/libraries unless a location is provided.");
  }
}

/**
 * Implements drush_MODULE_post_pm_enable().
 */
function drush_perfbar_post_pm_enable() {
  $modules = func_get_args();
  if (in_array('perfbar', $modules)) {
    drush_perfbar_plugin();
  }
}

/**
 * Callback for the drush command to download the perfBar library.
 */
function drush_perfbar_plugin() {
  if (!drush_shell_exec('type unzip')) {
    return drush_set_error(dt('Missing dependency: unzip. Install it before using this command.'));
  }
  $args = func_get_args();
  if ($args[0]) {
    $path = $args[0];
  }
  else {
    $path = 'sites/all/libraries';
  }
  $olddir = getcwd();
  if (file_exists($path)) {
    chdir($path);
    drush_log('Switched to Libraries directory');
  }
  else {
    mkdir($path);
    chdir($path);
    drush_log('Created and moved to libraries');
  }

  $filename = basename(PERFBAR_LIBRARY_URI);
  drush_log($filename);
  $dirname = 'perfbar';
  // Remove any existing perfbar plugin directory.
  if (is_dir($dirname)) {
    drush_log(dt('An existing version of the perfbar library was overwritten at @path', array('@path' => $path)), 'notice');
    $objects = scandir($dirname);
    foreach ($objects as $object) {
      if ($object != "." && $object != "..") {
        if (filetype($dirname . "/" . $object) !== 'dir') {
          unlink($dirname . "/" . $object);
        }
        else {
          rmdir($dirname . "/" . $object);
        }
      }
    }
    reset($objects);
    rmdir($dirname);
  }
  // Remove any existing perfbar plugin zip archive.
  if (is_file($filename)) {
    drush_op('unlink', $filename);
  }
  // Download the zip archive.
  if (!drush_shell_exec('wget ' . PERFBAR_LIBRARY_URI)) {
    drush_shell_exec('curl -O ' . PERFBAR_LIBRARY_URI);
  }
  if (is_file($filename)) {
    // Decompress the zip archive.
    drush_shell_exec('unzip -qq -o ' . $filename);
    // Remove the zip archive.
    drush_op('unlink', $filename);
    drush_log('Unzipped the file.');
  }
  if (file_exists('perfBar-0.1.0')) {
    rename('perfBar-0.1.0', 'perfbar');
  }
  // Set working directory back to the previous working directory.
  chdir($olddir);
  if (is_dir($path . '/' . $dirname)) {
    drush_log(dt('The perfBar library has been downloaded to @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to download the perfBar library to @path', array('@path' => $path)), 'error');
  }
}
