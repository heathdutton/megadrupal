<?php

/**
 * @file
 * Drush commands.
 */

/**
 * The Eventbrite PHP download URL.
 */
define('EVENTBRITE_PHP_URI', 'https://raw.github.com/ryanjarvinen/eventbrite.php/master/Eventbrite.php');
define('EVENTBRITE_PHP_PATH', 'sites/all/libraries');
define('EVENTBRITE_PHP_DIR', 'eventbrite');

/**
 * Implementation of hook_drush_command().
 */
function eventbrite_api_drush_command() {
  $items = array();
  $items['eventbrite-php'] = array(
    'description' => dt('Download and install the Eventbrite PHP file.'),
    'drupal dependencies' => array('libraries'),
    'arguments' => array(
      'path' => 'Optional. The path the library should be saved to. The default is "sites/all/libraries".',
    ),
    'examples' => array(
      'drush eventbrite-php sites/all/libraries' => 'Downloads the Eventbrite PHP library.',
      'drush ephp' => 'Downloads the Eventbrite PHP library.',
    ),
    'aliases' => array('ephp'),
  );
  return $items;
}

/**
 * Fetches the Eventbrite PHP file and saves it into the libraries.
 */
function drush_eventbrite_api_eventbrite_php($path = NULL) {
  // If a path isn't given, use the default.
  if (!$path) {
    $path = EVENTBRITE_PHP_PATH;
  }
  // Create the path if it does not exist.
  if (!is_dir($path)) {
    drush_op('mkdir', $path);
    drush_log(dt('Directory @path was created', array('@path' => $path)), 'notice');
  }
  // Set the directory to the libraries path.
  $olddir = getcwd();
  chdir($path);
  // Make the actual Eventbrite library directory.
  $dir = EVENTBRITE_PHP_DIR;
  drush_op('mkdir', $dir);
  // Make sure it can be opened.
  chmod($dir, 0644);
  // Download the file as eventbrite.php.
  drush_download_file(EVENTBRITE_PHP_URI, $dir . '/eventbrite.php');
  // Check if it worked.
  if (is_dir($dir)) {
    drush_log(dt('Eventbrite PHP has been installed in @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to install the Eventbrite PHP to @path', array('@path' => $path)), 'error');
  }
  // Set working directory back to the previous working directory.
  chdir($olddir);
}

/**
 * Implementation of hook_drush_help().
 */
function eventbrite_api_drush_help($section) {
  switch ($section) {
    case 'drush:eventbrite-php':
      return dt('Download and install the Eventbrite PHP file, default location is sites/all/libraries.');
  }
}
