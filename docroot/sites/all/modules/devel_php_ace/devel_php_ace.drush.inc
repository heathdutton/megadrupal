<?php
/**
 * @file
 * Download ACE Editor library with drush.
 */

/**
 * Implements hook_drush_command().
 */
function devel_php_ace_drush_command() {
  $items['dl-ace'] = array(
    'callback' => 'devel_php_ace_drush_download',
    'description' => dt('Downloads required ACE Editor library.'),
  );

  return $items;
}

/**
 * ACE Editor download callback.
 */
function devel_php_ace_drush_download() {
  $library = libraries_load('ace');
  $libraries_path = 'sites/all/libraries';
  $path = 'sites/all/libraries/ace';
  $url = $library['download url'];

  // Download and extract library.
  if (($tmp = drush_download_file($url)) && drush_tarball_extract($tmp, $libraries_path) && drush_op('rename', $libraries_path . '/ace-builds-master', $path)) {
    drush_log(dt('ACE Editor has been downloaded to @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to download ACE Editor to @path', array('@path' => $path)), 'error');
  }
}
