<?php

/**
 * @file
 * Drush integration for Justified module.
 */

/**
 * The Justified library URL.
 */
define('JUSTIFIED_DOWNLOAD_URL', 'https://github.com/nitinhayaran/Justified.js/archive/master.zip');

/**
 * Implements hook_drush_command().
 */
function justified_drush_command() {
  $items['justified-download'] = array(
    'description' => dt('Download and install Justified library.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );
  return $items;
}

/**
 * Command to download the Justified library.
 */
function drush_justified_download() {

  $libraries_dir = 'sites/all/libraries';
  $temp_justified_dir = $libraries_dir . '/Justified.js-master';
  $archive_path = DRUPAL_ROOT . '/' . $libraries_dir . '/justified-master.zip';

  if (!is_dir($libraries_dir)) {
    return drush_set_error('JUSTIFIED', dt('Directory !libraries_dir does not exitst.', array('!libraries_dir' => $libraries_dir)));
  }

  if (is_dir(JUSTIFIED_LIBRARY_DIR)) {
    if (drush_confirm(dt('Install location !justified_dir already exists. Do you want to overwrite it?', array('!justified_dir' => JUSTIFIED_LIBRARY_DIR)))) {
      drush_delete_dir(JUSTIFIED_LIBRARY_DIR, TRUE);
    }
    else {
      return drush_log(dt('Aborting.'));
    }
  }

  if (is_dir($temp_justified_dir)) {
    drush_delete_dir($temp_justified_dir, TRUE);
  }

  drush_shell_exec('wget --timeout=5 -O %s %s', $archive_path, JUSTIFIED_DOWNLOAD_URL);
  if (!drush_file_not_empty($archive_path)) {
    return drush_set_error('JUSTIFIED', dt('Could not download Justified library from GitHub.'));
  }
  if (!drush_tarball_extract($archive_path)) {
    return drush_set_error('JUSTIFIED', dt('Could not extract files from archive.'));
  }
  if (!drush_move_dir($temp_justified_dir, JUSTIFIED_LIBRARY_DIR, TRUE)) {
    return drush_set_error('JUSTIFIED', dt('Could not move justified-master directory.'));
  }

  file_unmanaged_delete($archive_path);

  if (is_dir(JUSTIFIED_LIBRARY_DIR)) {
    drush_log(dt('Justified library has been installed in @path', array('@path' => JUSTIFIED_LIBRARY_DIR)), 'success');

    // We do not need demo directory.
    drush_delete_dir(JUSTIFIED_LIBRARY_DIR . '/demo');
  }
  else {
    return drush_set_error('JUSTIFIED', dt('Drush was unable to install the Justified library.'));
  }

}
