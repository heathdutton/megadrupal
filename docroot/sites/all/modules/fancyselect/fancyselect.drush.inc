<?php

/**
 * @file
 * Drush integration for fancySelect.
 */

/**
 * The fancySelect plugin URI.
 */
define('FANCYSELECT_URI', 'https://github.com/octopuscreative/FancySelect/archive/master.zip');

/**
 * Implements hook_drush_command().
 */
function fancyselect_drush_command() {
  $items = array();

  // The key in the $items array is the name of the command.
  $items['fancyselect-plugin'] = array(
    'callback' => 'drush_fancyselect_plugin',
    'description' => dt('Download and install the fancySelect plugin.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'path' => dt('Optional. A path where to install the fancySelect plugin. If omitted Drush will use the default location.'),
    ),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function fancyselect_drush_help($section) {
  switch ($section) {
    case 'drush:fancyselect-plugin':
      return dt('Download and install the fancySelect plugin from https://github.com/octopuscreative/FancySelect, default location is sites/all/libraries.');
  }
}

/**
 * Command to download the fancySelect plugin.
 */
function drush_fancyselect_plugin() {
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
  if ($filepath = drush_download_file(FANCYSELECT_URI)) {
    $filename = basename($filepath);

    // Remove any existing fancySelect plugin directory.
    if (is_dir('fancyselect')) {
      drush_delete_dir('fancyselect', TRUE);
      drush_log(dt('A existing fancySelect plugin was deleted from @path', array('@path' => $path)), 'notice');
    }

    // Decompress the zip archive.
    drush_tarball_extract($filename);

    // Change the directory name to "fancyselect".
    drush_move_dir('FancySelect-master', 'fancyselect', TRUE);
  }

  if (is_dir('fancyselect')) {
    drush_log(dt('fancySelect plugin has been installed in @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to install the fancySelect plugin to @path', array('@path' => $path)), 'error');
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);
}
