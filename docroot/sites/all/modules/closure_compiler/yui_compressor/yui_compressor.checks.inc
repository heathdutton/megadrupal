<?php

/**
 * @file
 * Contains all local-compile checking functions.
 */

/**
 * Checks the availability of the local compilation.
 *
 * If the local compiler check fails, an error message is displayed and the
 * YUI Compressor is disabled.
 */
function yui_compressor_confirm_local($display_message = FALSE) {
  if (!yui_compressor_local_works()) {
    if ($display_message) {
      drupal_set_message(t('YUI Compressor unavailable. Please check the errors with local compiling below.'), 'error');
    }
    else {
      // If we are not displaying the message, we are writing to watchdog.
      watchdog('yui_compressor', 'Error: Local compiling failed.', WATCHDOG_ERROR);
    }
  }
}

/**
 * Test that the local closure compiler function works correctly.
 */
function yui_compressor_local_works() {
  // If performed this check during this execution, do not test again.
  static $result;
  if (isset($result)) {
    return $result;
  }

  module_load_include('inc', 'yui_compressor', 'yui_compressor.defines');

  // Try to create the file.
  $test_file_path = drupal_tempnam(file_directory_temp(), 'jscc');
  $test_string = 'var i = 4; alert(i+i);';
  $test_file_path = file_unmanaged_save_data($test_string, $test_file_path, FILE_EXISTS_REPLACE);
  // If file creation failed, we cannot compile locally.
  if (!$test_file_path) {
    $result = FALSE;
    return $result;
  }

  // Try to compile the file locally.
  $compiler_path = drupal_get_path('module', 'yui_compressor') . '/' . YUI_COMPRESSOR_FILE;
  $command = 'java -jar ' . $compiler_path . ' --type js ' . $test_file_path;
  $compiled_string = shell_exec($command);

  // Clean up.
  if (is_file($test_file_path)) {
    file_unmanaged_delete($test_file_path);
  }
  else {
    $result = FALSE;
    return $result;
  }

  // Check the compiled result exists.
  if (!$compiled_string || !strlen($compiled_string)) {
    $result = FALSE;
    return $result;
  }

  // Compilation should remove the whitespace, resulting in a lower char count.
  $result = strlen($compiled_string) < strlen($test_string);
  return $result;
}

/**
 * Tests whether compiling locally is operational.
 */
function yui_compressor_local_available() {
  module_load_include('inc', 'closure_compiler', 'closure_compiler.checks');
  return closure_compiler_java_installed() && yui_compressor_compiler_exists() && yui_compressor_local_works();
}

/**
 * Tests whether the local compiler is available.
 */
function yui_compressor_compiler_exists() {
  module_load_include('inc', 'yui_compressor', 'yui_compressor.defines');
  return file_exists(drupal_get_path('module', 'yui_compressor') . '/' . YUI_COMPRESSOR_FILE);
}
