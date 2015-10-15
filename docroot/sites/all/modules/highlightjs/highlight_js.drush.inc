<?php

/**
 * @file
 *   drush integration for highlightjs.
 */

/**
 * The highlightjs plugin URI.
 */
define('HIGHLIGHTJS_DOWNLOAD_URI', 'https://github.com/components/highlightjs/archive/master.zip');

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
function highlight_js_drush_command() {
  $items = array();

  // The key in the $items array is the name of the command.
  $items['highlightjs-plugin'] = array(
    'callback' => 'drush_highlightjs_plugin',
    'description' => dt('Download and install the highlight js plugin.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap.
    'arguments' => array(
      'path' => dt('Optional. A path where to install the highlight js plugin. If omitted Drush will use the default location.'),
    ),
    'aliases' => array('highlightjs'),
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
function highlight_js_drush_help($section) {
  switch ($section) {
    case 'drush:highlightjs-plugin':
      return dt('Download and install the highlight js plugin, default location is sites/all/libraries.');
  }
}

/**
 * Command to download the highlightjs plugin.
 */
function drush_highlightjs_plugin() {
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
  if ($filepath = drush_download_file(HIGHLIGHTJS_DOWNLOAD_URI)) {
    $filename = basename($filepath);
    $dirname =  'highlightjs-master';

    // Remove any existing highlightjs plugin directory.
    if (is_dir($dirname) || is_dir('highlightjs')) {
      drush_delete_dir($dirname, TRUE);
      drush_delete_dir('highlightjs', TRUE);
      drush_log(dt('A existing highlight js plugin was deleted from @path', array('@path' => $path)), 'notice');
    }

    // Decompress the zip archive.
    drush_tarball_extract($filename);

    // Change the directory name to "highlightjs" if needed.
    if ($dirname != 'highlightjs') {
      drush_move_dir($dirname, 'highlightjs', TRUE);
      $dirname = 'highlightjs';
    }
  }

  if (is_dir($dirname)) {
    drush_log(dt('highlight js plugin has been installed in @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to install the highlightjs plugin to @path', array('@path' => $path)), 'error');
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);
}
