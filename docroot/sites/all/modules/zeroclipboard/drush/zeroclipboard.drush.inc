<?php

/**
 * @file
 *   drush integration for zeroclipboard.
 */

/**
 * The zeroclipboard plugin URI.
 */
define('ZEROCLIPBOARD_DOWNLOAD_URI', 'https://github.com/zeroclipboard/zeroclipboard/archive/master.zip');

/**
 * Implementation of hook_drush_command().
 *
 * In this hook, you specify which commands your
 * drush module makes available, what it does and
 * description.
 *
 * Notice how this structure closely resembles how
 * you define menu hooks. See drush_parse_command()
 * for a list of recognized keys.
 *
 * @return
 *   An associative array describing your command(s).
 */
function zeroclipboard_drush_command() {
  $items = array();

  // the key in the $items array is the name of the command.
  $items['zeroclipboard-plugin'] = array(
    'callback' => 'drush_zeroclipboard_plugin',
    'description' => dt("Downloads the zeroclipboard plugin."),
    'arguments' => array(
      'path' => dt('Optional. A path where to install the zeroclipboard plugin. If omitted Drush will use the default location.'),
    ),
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
function zeroclipboard_drush_help($section) {
  switch ($section) {
    case 'drush:zeroclipboard-plugin':
      return dt("Downloads the zeroclipboard plugin from GitHub, default location is sites/all/libraries.");
  }
}

/**
 * Example drush command callback.
 *
 * This is where the action takes place.
 *
 * In this function, all of Drupals API is (usually) available, including
 * any functions you have added in your own modules/themes.
 *
 * To print something to the terminal window, use drush_print().
 *
 * @TODO Test me!
 */
function drush_zeroclipboard_plugin() {
  $file_path = drush_download_file(ZEROCLIPBOARD_DOWNLOAD_URI);
  if (!$file_path) {
    return drush_set_error(dt('Failed to download ZIP file.'));
  }

  // Setup the destination path to extract the ZIP file
  $args = func_get_args();
  if ($args[0]) {
    $path = $args[0];
  }
  else {
    $path = 'sites/all/libraries';
  }

  // EXTRAAAAACT!
  if (drush_tarball_extract($file_path, $path)) {
    drush_log(dt('zeroclipboard plugin has been downloaded to @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to download the zeroclipboard plugin to @path', array('@path' => $path)), 'error');
  }
}
