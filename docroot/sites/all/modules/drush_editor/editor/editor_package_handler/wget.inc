<?php

/**
 * @file
 * Drush integration for WYSIWYG editors.
 */

/**
 * Validate this package handler can run.
 *
 * @return bool
 *   Validated or Failed.
 */
function drush_editor_package_handler_validate() {
  // Check wget or curl command exists. Disable possible output.
  $debug = drush_get_context('DRUSH_DEBUG');
  drush_set_context('DRUSH_DEBUG', FALSE);

  $success = drush_shell_exec('wget --version');

  if (!$success) {
    $success = drush_shell_exec('curl --version');

    // Old version of curl shipped in darwin returns error status for --version
    // and --help. Give the chance to use it.
    if (!$success) {
      $success = drush_shell_exec('which curl');
    }
  }

  drush_set_context('DRUSH_DEBUG', $debug);

  if (!$success) {
    return drush_set_error('DRUSH_SHELL_COMMAND_NOT_FOUND', dt('wget nor curl executables found.'));
  }

  return TRUE;
}

/**
 * Download editor using wget or curl.
 *
 * @param array $request
 *   List of information.
 *
 * @return mixed
 *   Path to downloaded directory or FALSE.
 */
function drush_editor_package_handler_download_editor($request) {
  $editor = $request['editor'];
  $file   = $editor->download();

  if (!$file || !drush_file_is_tarball($file)) {
    return FALSE;
  }

  $tmpdir = drush_trim_path(drush_find_tmp()) . DIRECTORY_SEPARATOR . 'editor';

  if (is_dir($tmpdir)) {
    drush_delete_dir($tmpdir, TRUE);
  }

  drush_mkdir($tmpdir, TRUE);

  $file_list = drush_tarball_extract($file, $tmpdir, TRUE);

  return empty($file_list) ? FALSE : drush_trim_path($tmpdir . DIRECTORY_SEPARATOR . $editor->getLibraryName());
}
