<?php

/**
 * @file
 * Drush integration for Devicons.
 */

/**
 * Implements hook_drush_command().
 *
 * In this hook, you specify which commands your
 * drush module makes available, what it does and
 * description.
 *
 * @See drush_parse_command() for a list of recognized keys.
 */
function devicons_drush_command() {
  $items = array();

  $items['dl-devicons'] = array(
    'callback' => 'devicons_drush_download',
    'description' => dt('Downloads the required Devicons from project page.'),
  );
  return $items;
}


/**
 * Implements hook_drush_help().
 *
 * This function is called whenever a drush user calls
 * 'drush help <name-of-your-command>'
 */
function devicons_drush_download_drush_help($section) {
  switch ($section) {
    case 'drush:dl-devicons':
      return dt("Downloads the required Devicons from project page.");
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
 */
function devicons_drush_download() {

  $path = 'sites/all/libraries';

  // Create the path if it does not exist.
  if (!is_dir($path)) {
    drush_op('mkdir', $path);
    drush_log(dt('Directory @path was created', array('@path' => $path)), 'notice');
  }

  drush_op('chdir', $path);

  // Download the plugin.
  if (drush_shell_exec("git clone https://github.com/vorillaz/devicons.git")) {
    drush_log(dt('Devicons has been downloaded to @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to download Devicons to @path', array('@path' => $path)), 'error');
  }
}
