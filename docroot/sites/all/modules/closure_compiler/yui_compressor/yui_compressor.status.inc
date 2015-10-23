<?php

/**
 * @file
 * Contains status-related functions.
 */

/**
 * Adds status messages to the status page.
 */
function _yui_compressor_requirements_status() {
  module_load_include('inc', 'yui_compressor', 'yui_compressor.defines');
  module_load_include('inc', 'yui_compressor', 'yui_compressor.checks');
  module_load_include('inc', 'closure_compiler', 'closure_compiler.checks');

  $errors = array();
  $t = get_t();

  // Local compiler status.
  if (!closure_compiler_java_installed()) {
    $errors[] = $t('YUI Compressor: Java is either not locally installed, or not available to use from PHP');
  }
  if (!yui_compressor_compiler_exists()) {
    $errors[] = $t('YUI Compressor: The local compiler %compiler is not available, please install it to %directory', array(
      '%compiler' => YUI_COMPRESSOR_FILE,
      '%directory' => drupal_get_path('module', 'yui_compressor'),
    ));
  }
  if (!yui_compressor_local_works()) {
    $errors[] = $t('YUI Compressor: Local compilation failed, please check your Java version and file permissions');
  }

  return $errors;
}
