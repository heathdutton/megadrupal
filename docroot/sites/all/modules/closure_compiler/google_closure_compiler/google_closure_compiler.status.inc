<?php

/**
 * @file
 * Contains status-related functions.
 */

/**
 * Adds status messages to the status page.
 */
function _google_closure_compiler_requirements_status() {
  module_load_include('inc', 'google_closure_compiler', 'google_closure_compiler.defines');
  module_load_include('inc', 'google_closure_compiler', 'google_closure_compiler.checks');
  module_load_include('inc', 'closure_compiler', 'closure_compiler.defines');
  module_load_include('inc', 'closure_compiler', 'closure_compiler.checks');

  $errors = array();
  $t = get_t();

  // Local compiler status (if this method is selected).
  if (CLOSURE_COMPILER_LOCAL == variable_get('google_closure_compiler_process_method', CLOSURE_COMPILER_SEND_CONTENTS)) {
    if (!closure_compiler_java_installed()) {
      $errors[] = $t('Closure Compiler: Java is either not locally installed, or not available to use from PHP');
    }
    if (!google_closure_compiler_compiler_exists()) {
      $errors[] = $t('Closure Compiler: The local compiler %compiler is not available, please install it to %directory', array(
        '%compiler' => GOOGLE_CLOSURE_COMPILER_FILE,
        '%directory' => drupal_get_path('module', 'google_closure_compiler'),
      ));
    }
    if (!google_closure_compiler_local_works()) {
      $errors[] = $t('Closure Compiler: Local compilation failed, please check your Java version and file permissions');
    }
  }

  return $errors;
}
