<?php

/**
 * @file
 * Drush integration for PHP Console.
 */

/**
 * The PHP Console library URL.
 */
define('PC_DOWNLOAD_URL', 'https://github.com/barbushin/php-console/archive/master.zip');

/**
 * Implements hook_drush_command().
 */
function pc_drush_command() {
  $items['pc-download'] = array(
    'description' => dt('Download and install PHP Console library.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'aliases' => array('pc-d'),
  );
  return $items;
}


/**
 * Command to download the PHP Console library.
 */
function drush_pc_download() {

  $libraries_dir = 'sites/all/libraries';
  $pc_dir = $libraries_dir . '/php-console';
  $temp_pc_dir = $libraries_dir . '/php-console-master';
  $archive_path = DRUPAL_ROOT . '/' . $libraries_dir . '/php-console-master.zip';

  if (!is_dir($libraries_dir)) {
    return drush_set_error('PC', dt('Directory !libraries_dir does not exitst.', array('!libraries_dir' => $libraries_dir)));
  }

  if (is_dir($pc_dir)) {
    if (drush_confirm(dt('Install location !pc_dir already exists. Do you want to overwrite it?', array('!pc_dir' => $pc_dir)))) {
      drush_delete_dir($pc_dir, TRUE);
    }
    else {
      return drush_log(dt('Aborting.'));
    }
  }

  if (is_dir($temp_pc_dir)) {
    drush_delete_dir($temp_pc_dir, TRUE);
  }

  drush_shell_exec('wget --timeout=5 -O %s %s', $archive_path, PC_DOWNLOAD_URL);
  if (!drush_file_not_empty($archive_path)) {
    return drush_set_error('PC', dt('Could not download PHP Console library from GitHub.'));
  }
  if (!drush_tarball_extract($archive_path)) {
    return drush_set_error('PC', dt('Could not extract files from archive.'));
  }
  if (!drush_move_dir($temp_pc_dir, $pc_dir, TRUE)) {
    return drush_set_error('PC', dt('Could not move php-console-master directory.'));
  }

  file_unmanaged_delete($archive_path);

  // Protect the library from direct access.
  file_create_htaccess($pc_dir);

  if (is_dir($pc_dir)) {
    drush_log(dt('PHP Console library has been installed in @path', array('@path' => $pc_dir)), 'success');
  }
  else {
    return drush_set_error('PC', dt('Drush was unable to install the PHP Console library.'));
  }

}
