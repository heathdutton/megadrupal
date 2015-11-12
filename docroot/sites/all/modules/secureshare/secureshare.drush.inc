<?php
/**
 * @file
 * secureshare drush integration for lib downloading
 */
define('SECURESHARE_DOWNLOAD_URI', 'http://www.heise.de/extras/socialshareprivacy/jquery.socialshareprivacy.zip');
define('SECURESHARE_DEFAULT_DESTINATION', 'sites/all/libraries');
define('SECURESHARE_DEFAULT_DESTINATION_PATH', 'socialshareprivacy');

/**
 * Implementation of hook_drush_command().
 */
function secureshare_drush_command() {
  $items = array();
  // the key in the $items array is the name of the command.
  $items['secureshare-download'] = array(
    'description' => dt("Downloads the Secureshare / Socialshareprivacy plugin from Heise."),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap.
    'arguments' => array(
      'path' => dt('Optional. A path where to install the Secureshare plugin. If omitted Drush will use the default location.'),
    ),
    'core' => array('7+'),
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
function secureshare_drush_help($section) {
  switch ($section) {
    case 'drush:secureshare-plugin':
      return dt("Downloads the Heise Socialshare Privacy plugin from heise.de, default location is sites/all/libraries.");
  }
}

/**
 * Drush command callback
 * to download the Secureshare plugin.
 */
function drush_secureshare_download() {
  if (!drush_shell_exec('type unzip')) {
    return drush_set_error(dt('Missing dependency: unzip. Install it before using this command.'));
  }

  $args = func_get_args();
  if (!empty($args[0])) {
    $path = $args[0];
  }
  else {
    $path = SECURESHARE_DEFAULT_DESTINATION;
  }

  // Create the path if it does not exist.
  if (!is_dir($path)) {
    drush_op('mkdir', $path);
    drush_log(dt('Directory @path was created', array('@path' => $path)), 'notice');
  }

  // Set the directory to the download location.
  $olddir = getcwd();
  chdir($path);

  $filename = basename(SECURESHARE_DOWNLOAD_URI);

  // Remove any existing plugin directory
  if (is_dir(SECURESHARE_DEFAULT_DESTINATION_PATH)) {
    drush_log(dt('A existing Secureshare plugin was overwritten at @path', array('@path' => $path)), 'notice');
  }
  // Remove any existing plugin zip archive
  if (is_file($filename)) {
    drush_op('unlink', $filename);
  }

  // Download the zip archive
  if (!drush_shell_exec('wget ' . SECURESHARE_DOWNLOAD_URI)) {
    drush_shell_exec('curl -O ' . SECURESHARE_DOWNLOAD_URI);
  }

  if (is_file($filename)) {
    // Decompress the zip archive
    drush_shell_exec('unzip -qq -o ' . $filename);
    // Remove the zip archive
    drush_op('unlink', $filename);
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);

  if (is_dir($path . '/' . SECURESHARE_DEFAULT_DESTINATION_PATH)) {
    drush_log(dt('Secureshare plugin has been downloaded to @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to download the Secureshare plugin to @path', array('@path' => $path)), 'error');
  }
}

/**
 * Implements drush_MODULE_post_COMMAND().
 */
function drush_secureshare_post_pm_enable() {
  $extensions = func_get_args();
  // Deal with comma delimited extension list.
  if (strpos($extensions[0], ',') !== FALSE) {
    $extensions = explode(',', $extensions[0]);
  }

  if (in_array('secureshare', $extensions) && !drush_get_option('skip')) {
    drush_secureshare_download();
  }
}