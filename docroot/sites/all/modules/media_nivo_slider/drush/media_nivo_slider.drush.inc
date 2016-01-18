<?php

/**
 * @file
 *   drush integration for media_nivo_slider.
 */

// The Nivo Slider download path.
define('MEDIA_NIVO_SLIDER_DOWNLOAD_PATH', 'http://dev7studios.com/downloads/46');

// The Nivo Slider file name
define('MEDIA_NIVO_SLIDER_FILE_NAME', 'nivo-slider.2.7.1.zip');

// The Nivo Slider directory name
define('MEDIA_NIVO_SLIDER_DIR_NAME', 'nivo-slider');

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
function media_nivo_slider_drush_command() {
  $items = array();

  // the key in the $items array is the name of the command.
  $items['media-nivo-slider-plugin'] = array(
    'callback' => 'drush_media_nivo_slider_plugin',
    'description' => dt("Downloads the Nivo Slider jQuery plugin."),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap.
    'aliases' => array('nivosliderplugin'),
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
function media_nivo_slider_drush_help($section) {
  switch ($section) {
    case 'drush:media-nivo-slider-plugin':
      return dt("Downloads the Nivo Slider jQuery plugin to sites/all/libraries.");
  }
}

/**
 * Command to download the Nivo Slider jQuery plugin.
 */
function drush_media_nivo_slider_plugin() {
  if (!drush_shell_exec('type unzip')) {
    return drush_set_error(dt('Missing dependency: unzip. Install it before using this command.'));
  }

  $path = 'sites/all/libraries';

  // Create the path if it does not exist.
  if (!is_dir($path)) {
    drush_op('mkdir', $path);
    drush_log(dt('Directory @path was created', array('@path' => $path)), 'notice');
  }

  // Set the directory to the download location.
  $olddir = getcwd();
  chdir($path);

  $filename = basename(MEDIA_NIVO_SLIDER_DOWNLOAD_PATH);
  $dirname = MEDIA_NIVO_SLIDER_DIR_NAME;

  // Check for an existing Nivo Slider plugin folder
  if (is_dir($dirname)) {
    drush_log(dt('A existing Nivo Slider plugin was overwritten at @path', array('@path' => $path)), 'notice');
  }
  // Remove any existing Nivo Slider plugin zip archive
  if (is_file($filename)) {
    drush_op('unlink', $filename);
  }

  // Download the zip archive
  if (!drush_shell_exec('wget ' . MEDIA_NIVO_SLIDER_DOWNLOAD_PATH)) {
    drush_shell_exec('curl -O ' . MEDIA_NIVO_SLIDER_DOWNLOAD_PATH);
  }

  if (is_file($filename)) {
    // Decompress the zip archive
    drush_shell_exec('unzip -qq -o ' . $filename);
    // Remove the zip archive
    drush_op('unlink', $filename);
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);

  if (is_dir($path . '/' . $dirname)) {
    drush_log(dt('Nivo Slider plugin has been downloaded to @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to download the Nivo Slider plugin to @path', array('@path' => $path)), 'error');
  }
}
