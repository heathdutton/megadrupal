<?php

/**
 * @file
 * Contains all local-compile checking functions.
 */

/**
 * Checks the availability of the local compilation.
 *
 * If the preferred method is set to local and the local compiler check fails,
 * an error message is displayed and the method is set to the send content.
 */
function google_closure_compiler_confirm_local($display_message = FALSE) {
  module_load_include('inc', 'closure_compiler', 'closure_compiler.defines');

  if (CLOSURE_COMPILER_LOCAL == variable_get('google_closure_compiler_process_method', CLOSURE_COMPILER_SEND_CONTENTS) && !google_closure_compiler_local_works()) {
    if ($display_message) {
      drupal_set_message(t('Google Closure Compiler preferred method was set to use the API. Please check the errors with local compiling below.'), 'error');
    }
    else {
      // If we are not displaying the message, we are writing to watchdog.
      watchdog('google_closure_compiler', 'Error: Local compiling failed, switching to API mode.', WATCHDOG_ERROR);
    }
    variable_set('google_closure_compiler_process_method', CLOSURE_COMPILER_SEND_CONTENTS);
  }
}

/**
 * Test that the local closure compiler function works correctly.
 */
function google_closure_compiler_local_works() {
  // If performed this check during this execution, do not test again.
  static $result;
  if (isset($result)) {
    return $result;
  }

  module_load_include('inc', 'google_closure_compiler', 'google_closure_compiler.defines');

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
  $compiler_path = DRUPAL_ROOT . '/' . drupal_get_path('module', 'google_closure_compiler') . '/' . GOOGLE_CLOSURE_COMPILER_FILE;
  $command = 'java -jar ' . $compiler_path . ' --compilation_level ' . GOOGLE_CLOSURE_COMPILER_SIMPLE . ' --js "' . $test_file_path . '"';
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
function google_closure_compiler_local_available() {
  module_load_include('inc', 'closure_compiler', 'closure_compiler.checks');
  return closure_compiler_java_installed() && google_closure_compiler_compiler_exists() && google_closure_compiler_local_works();
}

/**
 * Tests whether the local compiler is available.
 */
function google_closure_compiler_compiler_exists() {
  module_load_include('inc', 'google_closure_compiler', 'google_closure_compiler.defines');
  return file_exists(drupal_get_path('module', 'google_closure_compiler') . '/' . GOOGLE_CLOSURE_COMPILER_FILE);
}
