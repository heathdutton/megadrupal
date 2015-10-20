<?php

/**
 * @file
 * Contains status-related functions.
 */

/**
 * Adds status messages to the status page.
 */
function _jsmin_php_requirements_status() {
  module_load_include('inc', 'jsmin_php', 'jsmin_php.checks');

  $errors = array();
  $t = get_t();

  // Local compiler status.
  if (!jsmin_php_compiler_exists()) {
    $errors[] = $t('JSMin-PHP: The local compiler %compiler is not available, please install it to %directory', array(
      '%compiler' => 'jsmin.php',
      '%directory' => 'sites/all/libraries/jsmin-php/',
    ));
  }
  if (!jsmin_php_local_works()) {
    $errors[] = $t('JSMin-PHP: Local compilation failed, please check your file permissions');
  }

  return $errors;
}
