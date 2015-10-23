<?php

/**
 * @file
 * Drush integration for Moneris API.
 */

/**
 * Implements hook_drush_command().
 */
function commerce_moneris_api_drush_command() {
  $items = array();
  $items['dl-moneris-api-us'] = array(
    'callback' => 'commerce_moneris_api_drush_download',
    'callback arguments' => array(
      'name' => 'Moneris API (US)',
      'download url' => 'https://raw.githubusercontent.com/maciejzgadzaj/moneris-api-us/master/mpgClasses.php',
      'destination' => 'moneris-api-us',
    ),
    'description' => dt('Downloads the required Moneris API library (US).'),
  );
  $items['dl-moneris-api-ca'] = array(
    'callback' => 'commerce_moneris_api_drush_download',
    'callback arguments' => array(
      'name' => 'Moneris API (CA)',
      'download url' => 'https://raw.githubusercontent.com/maciejzgadzaj/moneris-api-ca/master/mpgClasses.php',
      'destination' => 'moneris-api-ca',
    ),
    'description' => dt('Downloads the required Moneris API library (CA).'),
  );
  return $items;
}

/**
 * Implements hook_drush_help().
 */
function commerce_moneris_api_drush_help($section) {
  switch ($section) {
    case 'drush:dl-moneris-api-us':
      return dt("Downloads the required Moneris API library (US).");
    case 'drush:dl-moneris-api-ca':
      return dt("Downloads the required Moneris API library (CA).");
  }
}

/**
 * Drush command callback: download Moneris API library (US).
 */
function commerce_moneris_api_drush_download($library_name, $download_url, $destination) {
  // Make sure that unzip is available.
  if (!drush_shell_exec('type unzip')) {
    return drush_set_error('COMMERCE_MONERIS_API_UNZIP', dt('Missing dependency: unzip. Install it before using this command.'));
  }

  // Build destination path for download.
  if (module_exists('libraries')) {
    $path = 'sites/all/libraries/' . $destination;
  }
  else {
    $path = drupal_get_path('module', 'commerce_moneris_api') . '/includes/' . $destination;
  }

  // Create the destination path if it does not exist.
  if (!is_dir($path)) {
    if (!drush_shell_exec('mkdir -p  %s', $path)) {
      return drush_set_error('COMMERCE_MONERIS_API_MKDIR', dt('Drush was unable to create the library directory at @path.', array('@path' => $path)));
    }
  }

  // Change current working directory to destination path.
  drush_op('chdir', $path);

  // Remove previously download Moneris API library if exists.
  $filename = basename($download_url);
  if (is_file($filename)) {
    drush_op('unlink', $filename);
  }

  // Download the library file (skipping all example files).
  if (!drush_shell_exec('wget ' . $download_url)) {
    if (!drush_shell_exec('curl -O ' . $download_url)) {
      return drush_set_error('COMMERCE_MONERIS_API_WGET', dt('Drush was unable to download the @name library to @path.', array(
        '@name' => $library_name,
        '@path' => $path,
      )));
    }
  }

  drush_log(dt('@name library has been downloaded to @path.', array(
    '@name' => $library_name,
    '@path' => $path,
  )), 'success');
}
