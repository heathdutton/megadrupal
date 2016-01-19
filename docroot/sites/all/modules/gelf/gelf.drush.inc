<?php

/**
 * @file
 * Drush integration for the gelf module.
 */

/**
 * Implements hook_drush_command().
 */
function gelf_drush_command() {
  $items['gelf-download'] = array(
    'description' => dt('Downloads the GELF library from https://github.com/Graylog2/gelf-php.git.'),
    'arguments' => array(
      'path' => dt('Optional. A path to the download folder. If omitted Drush will use the sites/all/libraries/gelf-php.'),
    ),
    'aliases' => array('gelfd'),
  );
  return $items;
}

/**
 * Implementation of hook_drush_help().
 */
function gelf_drush_help($section) {
  switch ($section) {
    case 'drush:gelf-download':
      return dt('Downloads the library from https://github.com/Graylog2/gelf-php. Places it in the libraries directory. Skips download if library already present. This all happens automatically if you enable gelf using drush.');
  }
}

/**
 * A command callback. Download the GELF library using git.
 */
function drush_gelf_download() {
  $args = func_get_args();
  if (isset($args[0])) {
    $path = $args[0];
  }
  else {
    $path = drush_get_context('DRUSH_DRUPAL_ROOT');
    if (function_exists('libraries_get_path')) {
      $path .= '/' . libraries_get_path('gelf-php');
    }
  }

  if (is_dir($path)) {
    drush_log(' already present. No download required.', 'ok');
  }
  elseif (drush_shell_cd_and_exec(dirname($path), 'git clone https://github.com/Graylog2/gelf-php.git')) {
    drush_log(dt('GELF has been cloned via git to @path.', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to clone to @path.', array('@path' => $path)), 'error');
  }
}

/**
 * Implements drush_MODULE_post_COMMAND().
 */
function drush_gelf_post_pm_enable() {
  $modules = func_get_args();
  if (in_array('gelf', $modules)) {
    drush_gelf_download();
  }
}
