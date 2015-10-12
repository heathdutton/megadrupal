<?php

/**
 * @file
 * Drush integration for the piecemaker module.
 */

/**
 * Implements hook_drush_command().
 */
function piecemaker_drush_command() {
  $items['piecemaker-download'] = array(
    'description' => dt('Downloads the Piecemaker library from https://github.com/arcaneadam/Piecemaker-Drupal.'),
    'aliases' => array('pmkdl'),
  );
  return $items;
}

/**
 * Implementation of hook_drush_help().
 */
function piecemaker_drush_help($section) {
  switch ($section) {
    case 'drush:piecemaker-download':
      return dt('Downloads the Piecemaker library from https://github.com/arcaneadam/Piecemaker-Drupal. Places it in the libraries directory. Skips download if library already present. This all happens automatically if you enable piecemaker using drush.');
  }
}

/**
 * A command callback. Download the Piecemaker library using git.
 */
function drush_piecemaker_download() {
  $path = drush_get_context('DRUSH_DRUPAL_ROOT');

  $path .= '/'. libraries_get_path('piecemaker');
  // If we have the libraries module go ahead and make the path.
  if (!is_dir($path)) {
    drush_mkdir($path);
  }

  $version = '7.x-1.x';
  $url = "https://github.com/arcaneadam/Piecemaker-Drupal/zipball/{$version}";
  $filepath = $path . '/piecemaker.zip';

  if (file_exists($path . '/piecemaker.swf')) {
    drush_log('Piecemaker already present. No download required.', 'ok');
    return;
  }
  //Download the file
  if (!drush_shell_exec('curl -L -o %s ' . $url, $filepath)) {
    return drush_set_error('PIECEMAKER_FETCH', dt('Drush was unable to download the Piecemaker library to @path.', array('@path' => $filepath)));
  }
  // Unzip it the file -- using the "update" option to avoid being prompted
  // about overwriting files.
  if (!drush_shell_exec("unzip -u {$filepath} -d {$path}")) {
    return drush_set_error('PIECEMAKER_UNTAR', dt('Drush was unable to untar the archive @path.', array('@path' => $filepath)));
  }
  if (!drush_shell_exec("mv {$path}/arcaneadam-Piecemaker-Drupal*/* {$path}")) {
    return drush_set_error('PIECEMAKER_MV', dt('Drush was unable to move the files to the appropriate directory'));
  }
  drush_shell_exec("rm -fR {$path}/arcaneadam-Piecemaker-Drupal*");

  drush_log(dt('Piecemaker library has been installed to @path.', array('@path' => $path)), 'success');
}

/**
 * Implements drush_MODULE_post_COMMAND().
 */
function drush_piecemaker_post_pm_enable() {
  $modules = func_get_args();
  if (in_array('piecemaker', $modules)) {
    drush_piecemaker_download();
  }
}