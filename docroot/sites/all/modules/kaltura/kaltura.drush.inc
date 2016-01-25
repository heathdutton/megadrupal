<?php

/**
 * @file
 * Drush integration for kaltura.
 */

/**
 * The Kaltura client library URI.
 */
define('KALTURA_DOWNLOAD_URI', 'http://cdnbakmi.kaltura.com/content/clientlibs/php5_16-12-2014.tar.gz');
define('KALTURA_DOWNLOAD_PREFIX', 'KalturaClient-');

/**
 * Implements hook_drush_command().
 */
function kaltura_drush_command() {
  $items['kaltura-client-install'] = array(
    'callback' => 'drush_kaltura_client_install',
    'description' => dt('Download and install the Kaltura client library.'),
    'bootstrap' => DRUSH_BOOTSTRAP_NONE,
    'arguments' => array(
      'path' => dt('Optional. A path where to install the Kaltura client library. If omitted Drush will use the default location.'),
    ),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function kaltura_drush_help($section) {
  switch ($section) {
    case 'drush:kaltura-client-insall':
      return dt('Download and install the Kaltura client library, default location is sites/all/libraries.');
  }
}

/**
 * Command to download and install the Kaltura client library.
 */
function drush_kaltura_client_install() {
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

  // Download the archive.
  if ($filepath = drush_download_file(KALTURA_DOWNLOAD_URI)) {
    $filename = basename($filepath);
    $dirname = KALTURA_DOWNLOAD_PREFIX . basename($filepath, '.tar.gz');

    // Remove any existing Kaltura client directory.
    if (is_dir($dirname) || is_dir('KalturaClient')) {
      drush_delete_dir($dirname, TRUE);
      drush_delete_dir('KalturaClient', TRUE);
      drush_log(dt('An existing Kaltura client library was deleted from @path', array('@path' => $path)), 'notice');
    }

    // Decompress the archive.
    drush_tarball_extract($filename, $dirname);

    // Change the directory to "KalturaClient" if needed.
    drush_move_dir($dirname . '/php5', 'KalturaClient', TRUE);
    drush_delete_dir($dirname, TRUE);
    $dirname = 'KalturaClient';
  }

  if (isset($dirname) && is_dir($dirname)) {
    drush_log(dt('Kaltura client library has been installed in @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to install the Kaltura client library to @path', array('@path' => $path)), 'error');
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);
}
