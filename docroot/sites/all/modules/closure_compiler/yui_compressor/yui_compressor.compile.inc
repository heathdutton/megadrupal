<?php

/**
 * @file
 * Contains the actual compilation functions.
 */

/**
 * Adds compilation parameters for Yahoo's YUI Compressor service.
 */
function _yui_compressor_compile_params() {
  return array();
}

/**
 * Compiles a single JS file.
 *
 * @param string $uri
 *   The location of the JS file to compile.
 * @param array $params
 *   An empty array.
 *
 * @return string
 *   The compilation result (FALSE or empty string on failure).
 */
function _yui_compressor_compile_file($uri, $params) {
  // Run the compilation.
  return _yui_compressor_compile_file_local($uri);
}

/**
 * Compiles a JS file locally.
 *
 * @param string $uri
 *   The location of the JS file to compile.
 *
 * @return string
 *   The compilation result (FALSE or empty string on failure).
 */
function _yui_compressor_compile_file_local($uri) {
  module_load_include('inc', 'yui_compressor', 'yui_compressor.defines');

  // The compiler script writes to a temporary file, prepends the signature
  // and replaces the original javascript file - the raw console output
  // can include error or warning messages, which breaks JS.
  $temp_file_path = drupal_tempnam(file_directory_temp(), 'jscc');
  $compiler_path = drupal_get_path('module', 'yui_compressor') . '/' . YUI_COMPRESSOR_FILE;
  $command = 'java -jar ' . $compiler_path . ' --type js -o ' . $temp_file_path . ' ' . drupal_realpath(check_plain($uri));
  shell_exec($command);

  // Check whether the compiled file contains any content.
  $compiled_js = file_get_contents($temp_file_path);
  if (!$compiled_js) {
    return FALSE;
  }

  // Cleanup.
  file_unmanaged_delete($temp_file_path);
  return $compiled_js;
}
