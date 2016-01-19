<?php
/**
 * @file
 * Internal API methods for the module_install module.
 */

/**
 * Helper to return a list of directories which will be searched.
 *
 * @param string $directory
 *   The subdirectory to include.
 *
 * @return array
 *   An array of directories which will be searched for possible destination
 *   candidate folders.
 */
function module_install_get_search_directories($directory) {
  $searchdir = array();

  // Always search sites/all/* as well as the global directories.
  $dir = 'sites/all/' . $directory;
  $searchdir[$dir] = $dir;

  // The 'profiles' directory contains pristine collections of modules and
  // themes as organized by a distribution. It is pristine in the same way
  // that /modules is pristine for core; users should avoid changing anything
  // there in favor of sites/all or sites/<domain> directories.
  $profiles = array();
  $profile = drupal_get_profile();

  // In case both profile directories contain the same extension, the actual
  // profile always has precedence.
  $profiles[] = $profile;
  foreach ($profiles as $profile) {
    $dir = 'profiles/' . $profile . '/' . $directory;
    if (file_exists($dir)) {
      $searchdir[$dir] = $dir;
    }
  }
  $config = conf_path();
  $dir = $config . '/' . $directory;
  if (file_exists($dir)) {
    $searchdir[$dir] = $dir;
  }

  return $searchdir;
}

/**
 * Helper to scan a directory and look for candidate destination folders.
 *
 * @param string $dir
 *   The directory to scan
 *
 * @param unknown $found
 *   An array containing candidate destination folders
 */
function module_install_get_destination_directories($dir, &$found, $type) {

  $exceptions = module_invoke_all('module_install_exceptions');

  // Get directories automatically.
  if ($handle = opendir($dir)) {
    while (false !== ($file = readdir($handle))) {
      if (is_dir($dir . '/' . $file) && $file != '.' && $file != '..') {
        $directory = $dir . '/' . $file;
        $files = file_scan_directory($directory, '/.*\.module$/', array(
          'recurse' => false
        ));
        if (count($files) == 0) {
          if (!in_array($directory, $found) && !in_array($directory, $exceptions)) {
            $found[$directory] = $directory;
          }
        }
      }
    }
    closedir($handle);
  }

  // Get specific listed directories.
  $includes = module_invoke_all('module_install_includes');
  foreach ($includes as $directory) {
    if (is_string($directory) && file_exists($directory) && !in_array($directory, $found)) {
      $found[$directory] = trim($directory, '/');
    }
  }

}

/**
 * Helper function to get the destination choice variable format.
 *
 * @return string
 *   The variable name.
 */
function module_install_get_destination_variable() {
  global $user;
  return 'module_install_destination_' . $user->uid;
}

/**
 * Helper function to get the destination choice variable.
 *
 * @return string
 *   The destination folder path.
 */
function module_install_get_destination_choice($default) {
  global $user;
  $variable = module_install_get_destination_variable();
  return variable_get($variable, $default);
}

/**
 * Helper function to set the destination choice variable.
 *
 * @param string $choice
 *   The destination folder path.
 */
function module_install_set_destination_choice($choice) {
  global $user;
  $variable = module_install_get_destination_variable();
  variable_set($variable, $choice);
}

/**
 * Helper function to delete the destination choice variable.
 */
function module_install_del_destination_choice() {
  global $user;
  $variable = module_install_get_destination_variable();
  variable_del($variable);
}
