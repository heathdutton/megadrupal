<?php

/**
 * @file
 * Drush integration for Splash Block.
 */

/**
 * The jStorage plugin URI.
 */
define('JSTORAGE_DOWNLOAD_URI', 'https://github.com/andris9/jStorage/zipball/master');

/**
 * Implements hook_drush_command().
 */
function splash_block_drush_command() {
  $items = array();

  // The key in the $items array is the name of the command.
  $items['sb-get-jstorage'] = array(
    'description' => dt("Downloads the jStorage library required by splash_block module."),
    'arguments' => array(
      'path' => dt('Optional. A path to install the jStorage library in. If omitted, Drush will use the default location.'),
    ),
    'aliases' => array('sbg'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function splash_block_drush_help($section) {
  switch ($section) {
    case 'drush:sb-get-jstorage':
      return dt("Downloads the jStorage library required by splash_block module (from github), default location is sites/all/libraries.");
  }
}

/**
 * Implements drush_MODULE_post_COMMAND().
 */
function drush_splash_block_post_pm_enable() {
  $extensions = func_get_args();
  // Deal with comma delimited extension list.
  if (strpos($extensions[0], ',') !== FALSE) {
    $extensions = explode(',', $extensions[0]);
  }

  if (in_array('splash_block', $extensions) && !drush_get_option('skip')) {
    drush_splash_block_sb_get_jstorage();
  }
}

/**
 * Commands to download the jStorage library.
 */
function drush_splash_block_sb_get_jstorage($path = 'sites/all/libraries') {
  if (!drush_shell_exec('type unzip')) {
    return drush_set_error(dt('Missing dependency: unzip. Install it before using this command.'));
  }

  // Create the path if it does not exist.
  if (!is_dir($path)) {
    drush_op('mkdir', $path);
    drush_log(dt('Directory @path was created', array('@path' => $path)), 'notice');
  }

  // Set the directory to the download location.
  $olddir = getcwd();
  chdir($path);

  $filename = basename(JSTORAGE_DOWNLOAD_URI);

  // Remove any existing jStorage library zip archive.
  if (is_file($filename)) {
    drush_op('unlink', $filename);
  }

  // Download the zip archive.
  if (!drush_shell_exec('wget ' . JSTORAGE_DOWNLOAD_URI)) {
    drush_shell_exec('curl -O ' . JSTORAGE_DOWNLOAD_URI);
  }

  if (is_file('master')) {
    // Decompress the zip archive.
    drush_shell_exec('unzip -qq -o master');
    // Remove the zip archive.
    drush_op('unlink', 'master');

    drush_shell_exec('mv andris9-jStorage* jstorage');
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);

  if (is_dir($path . '/jstorage')) {
    drush_log(dt('jStorage library has been downloaded to @path.', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to download the jStorage library to @path', array('@path' => $path)), 'error');
  }
}
