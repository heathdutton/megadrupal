<?php

/**
 * @file
 *   drush integration for seostats (download the library automatically).
 */

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
 * @See drush_parse_command() for a list of recognized keys.
 *
 * @return
 *   An associative array describing your command(s).
 */
function seostats_drush_command() {
  $items = array();

  $items['seostats-download'] = array(
    'callback' => 'seostats_drush_seostats_download',
    'description' => dt('Downloads the required SEO Stats library from https://github.com/eyecatchup/SEOstats'),
    'aliases' => array('seostatsdl'),
    'arguments' => array(
      'path' => dt('Optional. A path to the seostats module. If omitted Drush will use the default location.'),
    ),
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
function seostats_drush_help($section) {
  switch ($section) {
    case 'drush:seostats-download':
      return dt("Downloads the required SEO Stats library from https://github.com/eyecatchup/SEOstats. Include an optional path.");
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
 *
 */
function seostats_drush_seostats_download() {
  $args = func_get_args();
  if ($args[0]) {
    $path = $args[0];
  }
  else {
    $path = drush_get_context('DRUSH_DRUPAL_ROOT');
    if (module_exists('libraries')) {
      $path .= '/' . libraries_get_path('seostats');
    }
    else {
      $path .= '/' . drupal_get_path('module', 'seostats') . '/vendor';
    }
  }

  // Create the path if it does not exist yet.
  if (!is_dir($path)) {
    drush_mkdir($path);
  }

  if (is_dir($path . '/SEOstats')) {
    drush_log('SEOstats library already present. No download required.', 'ok');
  }
  elseif (drush_op('chdir', $path) &&
      drush_shell_exec('wget -O seostats.tar.gz http://github.com/eyecatchup/SEOstats/tarball/master') &&
      drush_shell_exec('tar zxvf seostats.tar.gz') &&
      drush_shell_exec('mv eyecatchup-SEOstats-* SEOstats') &&
      drush_shell_exec('rm seostats.tar.gz')) {
    drush_log(dt('The latest SEO Stats library has been downloaded to @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to download the SEO Stats library to @path', array('@path' => $path)), 'error');
  }
}

/**
 * Implements drush_MODULE_post_COMMAND().
 */
function drush_seostats_post_pm_enable() {
  $extensions = func_get_args();
  // Deal with comma delimited extension list.
  if (strpos($extensions[0], ',') !== FALSE) {
    $extensions = explode(',', $extensions[0]);
  }

  if (in_array('seostats', $extensions) && !drush_get_option('skip')) {
    seostats_drush_seostats_download();
  }
}
