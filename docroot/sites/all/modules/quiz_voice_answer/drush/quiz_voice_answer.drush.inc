<?php

/**
 * @file
 *   drush integration for Recordmp3js-Drupal.
 */

/**
 * The Recordmp3js-Drupal plugin URI.
 */
define('RECORDMP3JS_DRUPAL_DOWNLOAD_URI', 'https://github.com/Ruslan03492/Recordmp3js-Drupal/releases/download/1.0.0/Recordmp3js-Drupal_v1.0.0.zip');

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
function quiz_voice_answer_drush_command() {
  $items = array();

  // The key in the $items array is the name of the command.
  $items['recordmp3js-drupal-plugin'] = array(
    'callback' => 'drush_recordmp3js_drupal_plugin',
    'description' => dt('Download and install the Recordmp3js-Drupal plugin for quiz_voice_answer module.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap.
    'arguments' => array(
      'path' => dt('Optional. A path where to install the Recordmp3js-Drupal plugin. If omitted Drush will use the default location.'),
    ),
    'aliases' => array('rmp3plugin'),
  );

  return $items;
}

/**
 * Implementation of hook_drush_help().
 *
 * This function is called whenever a drush user calls
 * 'drush help <name-of-your-command>'.
 *
 * @param
 *   A string with the help section (prepend with 'drush:').
 *
 * @return
 *   A string with the help text for your command.
 */
function quiz_voice_answer_drush_help($section) {
  switch ($section) {
    case 'drush:recordmp3js-drupal-plugin':
      return dt('Download and install the Recordmp3js-Drupal plugin from https://github.com/Ruslan03492/Recordmp3js-Drupal, default location is sites/all/libraries.');
  }
}

/**
 * Command to download the Recordmp3js-Drupal plugin.
 */
function drush_recordmp3js_drupal_plugin() {
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
  if ($filepath = drush_download_file(RECORDMP3JS_DRUPAL_DOWNLOAD_URI)) {
    $filename = basename($filepath);
    $dirname =  basename($filepath, '.zip');

    // Remove any existing Chosen plugin directory.
    if (is_dir($dirname) || is_dir('Recordmp3js-Drupal')) {
      drush_delete_dir($dirname, TRUE);
      drush_delete_dir('Recordmp3js-Drupal', TRUE);
      drush_log(dt('A existing Recordmp3js-Drupal plugin was deleted from @path', array('@path' => $path)), 'notice');
    }

    // Decompress the zip archive.
    drush_tarball_extract($filename, $dirname);

    // Change the directory name to "Recordmp3js-Drupal" if needed.
    if ($dirname != 'Recordmp3js-Drupal') {
      drush_move_dir($dirname, 'Recordmp3js-Drupal', TRUE);
      $dirname = 'Recordmp3js-Drupal';
    }
  }

  if (is_dir($dirname)) {
    drush_log(dt('Recordmp3js-Drupal plugin has been installed in @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to install the Recordmp3js-Drupal plugin to @path', array('@path' => $path)), 'error');
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);
}
