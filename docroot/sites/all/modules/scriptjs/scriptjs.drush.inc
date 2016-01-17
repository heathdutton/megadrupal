<?php
/**
 * @file
 * Script.js Drush commands. 
 */

/**
 * Implements of hook_drush_command().
 */
function scriptjs_drush_command() {
  return array(
    'scriptjs-download' => array(
      'description' => 'Download the $script.js JavaScript library to your sites/all/libraries directory.',
      'aliases' => array('scriptjs-dl'),
    ),
  );
}

/**
 * Implements hook_drush_help().
 */
function scriptjs_drush_help($section) {
  switch ($section) {
    case 'drush:scriptjs-download':
      return dt('Download the $script.js JavaScript library to your sites/all/libraries directory.');
  }
}

/**
 * Drush command.
 * Downloads the $script.js javascript library to the sites/all/libraries directory.
 * 
 * @see scriptjs_drush_command().
 */
function drush_scriptjs_download() {
  // Check to see if Drush Make exists. It is part of Drush core in 5.0-rc2 and up.
  // We use Drush Make to avoid having to re-implement the download logic ourselves.
  // This will only work with Drush 5.0-rc2 or newer.
  // @link http://drupal.org/project/drush
  if (!function_exists('make_download_factory')) {
    drush_die(dt('Script.js requires a newer version of Drush. Please update Drush to 5.0-rc2 or newer: http://drupal.org/project/drush'));
  }

  $path = 'sites/all/libraries/scriptjs';

  if (!file_exists($path)) {
    // Try to create directory.
    $return = drush_op_system("mkdir -p {$path}");

    // No go, ask user to create directory manually.
    if ($return !== 0) {
      drush_die(dt('The directory "sites/all/libraries/scriptjs" does not exist and could not be created due to permissions. Please create it manually and try again.'));
    }
  }

  // Notify user we are starting to download the component.
  drush_print(dt("\nBeginning download of \$script.js library.\n"));

  // Create download array for Drush Make.
  $download = array(
    'type' => 'get',
    'url' => 'https://github.com/ded/script.js/tarball/master',
  );

  $dir = make_download_file('$script.js', $download, $path);

  // Display success message.
  drush_print(dt("\n\$script.js library successfully downloaded and installed to !dir.", array('!dir' => $dir)));
}