<?php
/**
 * @file
 * Documents API functions for module_install module.
 */

/**
 * Get subfolders to be excluded from the possible install directories.
 *
 * @return array
 *   An array of strings representing directories to be excluded from the
 *   install list. Directories are relative to the Drupal root.
 */
function hook_module_install_exceptions() {
  return array(
    'sites/all/modules/features',
  );
}

/**
 * Get subfolders to be included to the possible install directories.
 *
 * @return array
 *   An array of strings representing directories to be included to the install
 *   list. Directories are relative to the Drupal root.
 */
function hook_module_install_includes() {
  return array(
    'sites/all/modules',
  );
}
