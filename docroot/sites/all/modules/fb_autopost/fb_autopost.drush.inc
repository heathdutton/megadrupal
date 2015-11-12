<?php

/**
 * @file
 * drush integration for Facebook Autopost.
 */

/**
 * The Facebook PHP SDK URI.
 */
define('FBAUTOPOST_DOWNLOAD_URI', 'https://github.com/facebook/facebook-php-sdk/archive/master.zip');
define('FBAUTOPOST_DIRNAME', 'facebook-php-sdk');

/**
 * Implements hook_drush_command().
 */
function fb_autopost_drush_command() {
  $items = array();

  // The key in the $items array is the name of the command.
  $items['download-facebook-php-sdk'] = array(
    'callback' => 'drush_facebook_php_sdk',
    'description' => dt('Download and install the Facebook PHP SDK library.'),
    // No bootstrap.
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'path' => dt('Optional. A path where to install the Facebook PHP SDK library. If omitted Drush will use the default location.'),
    ),
    'aliases' => array('dl-fbsdk'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function fb_autopost_drush_help($section) {
  switch ($section) {
    case 'drush:facebook-php-sdk':
      return dt('Download and install the Facebook PHP SDK library from GitHub, default location is sites/all/libraries.');
  }
}

/**
 * Command to download the Facebook PHP SDK library.
 */
function drush_facebook_php_sdk() {
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

  $filename = basename(FBAUTOPOST_DOWNLOAD_URI);

  // Remove any existing Facebook PHP SDK library directory.
  if (is_dir(FBAUTOPOST_DIRNAME)) {
    drush_delete_dir(FBAUTOPOST_DIRNAME, TRUE);
    drush_log(dt('A existing Facebook PHP SDK library was deleted from @path', array('@path' => $path)), 'notice');
  }
  // Remove any existing Facebook PHP SDK library zip archive.
  if (is_file($filename)) {
    drush_delete_dir($filename, TRUE);
  }

  // Download the zip archive.
  drush_download_file(FBAUTOPOST_DOWNLOAD_URI);

  if (is_file($filename)) {
    // Decompress the zip archive.
    $extracted_files = drush_tarball_extract($filename, FALSE, TRUE);
    // Get the last extracted file and get the initial folder.
    $matches = array();
    if (preg_match("@^[^/]*/@", end($extracted_files), $matches)) {
      drush_move_dir(rtrim($matches[0], '/'), FBAUTOPOST_DIRNAME);
    }
  }

  if (is_dir(FBAUTOPOST_DIRNAME)) {
    drush_log(dt('Facebook PHP SDK library has been installed in @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to install the Facebook PHP SDK library to @path', array('@path' => $path)), 'error');
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);
}
