<?php

/**
 * @file
 * Drush integration for textcomplete.
 */

define('TEXTCOMPLETE_DOWNLOAD_URI', 'https://github.com/yuku-t/jquery-textcomplete/archive/master.zip');
define('TEXTCOMPLETE_DOWNLOAD_PREFIX', 'jquery-textcomplete-');

/**
 * Implements hook_drush_command().
 */
function textcomplete_drush_command() {
  $items = array();
  $items['textcomplete-plugin'] = array(
    'callback' => 'drush_textcomplete_plugin',
    'description' => dt('Download and install the Textcomplete plugin.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'path' => dt('Optional. A path where to install the Textcomplete plugin. If omitted Drush will use the default location.'),
    ),
    'aliases' => array('textcompleteplugin'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function textcomplete_drush_help($section) {
  switch ($section) {
    case 'drush:textcomplete-plugin':
      return dt('Download and install the Textcomplete plugin from github.com/yuku-t, default location is sites/all/libraries.');
  }
}

/**
 * Command to download the Textcomplete plugin.
 */
function drush_textcomplete_plugin() {
  $args = func_get_args();
  $result = FALSE;

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

  // Download the tarball archive.
  if ($filepath = drush_download_file(TEXTCOMPLETE_DOWNLOAD_URI)) {
    $filename = basename($filepath);
    $dirname = TEXTCOMPLETE_DOWNLOAD_PREFIX . basename($filepath, '.zip');

    // Remove any existing Geocomplete plugin directory.
    if (is_dir($dirname) || is_dir('textcomplete')) {
      drush_delete_dir($dirname, TRUE);
      drush_delete_dir('textcomplete', TRUE);
      drush_log(dt('An existing Textcomplete plugin was deleted from @path', array('@path' => $path)), 'notice');
    }

    // Decompress the zip archive.
    $result = drush_tarball_extract($filename);

    // Change the directory name to "textcomplete" if needed.
    if ($dirname != 'textcomplete') {
      $result = drush_move_dir($dirname, 'textcomplete', TRUE);
    }
  }

  if ($result) {
    drush_log(dt('Textcomplete plugin has been installed in @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to install the Textcomplete plugin to @path', array('@path' => $path)), 'error');
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);
}
