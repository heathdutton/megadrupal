<?php

/**
 * @file
 * Drush command for downloading the spring.js script for TNS Gallup.
 */

/**
 * Implements hook_drush_command().
 */
function tns_gallup_drush_command() {
  $items = array();

  $items['tns-gallup-download-libraries'] = array(
    'description' => 'Download the spring.js script for TNS Gallup.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );

  return $items;
}

/**
 * Download the libraries.
 */
function drush_tns_gallup_download_libraries() {
  // We will rely on `drush make` to do the download.
  $makefile = drupal_get_path('module', 'tns_gallup') . '/tns_gallup.make';

  $info = make_parse_info_file($makefile);
  $libraries = implode(',', array_keys($info['libraries']));

  drush_set_option('no-core', TRUE);
  drush_set_option('libraries', $libraries);
  drush_invoke('make', array($makefile, DRUPAL_ROOT));
}
