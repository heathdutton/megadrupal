<?php

/**
 * @file
 * Contains all local-compile checking functions.
 */

/**
 * Checks the availability of the local compilation.
 *
 * If the local compiler check fails, an error message is displayed and the
 * JSMin-PHP is disabled.
 */
function jsmin_php_confirm_local($display_message = FALSE) {
  if (!jsmin_php_local_works()) {
    if ($display_message) {
      drupal_set_message(t('JSMin-PHP unavailable. Please check the errors with local compiling below.'), 'error');
    }
    else {
      // If we are not displaying the message, we are writing to watchdog.
      watchdog('jsmin_php', 'Error: Local compiling failed.', WATCHDOG_ERROR);
    }
  }
}

/**
 * Test that the local jsmin-php function works correctly.
 */
function jsmin_php_local_works() {
  // If performed this check during this execution, do not test again.
  static $result;
  if (isset($result)) {
    return $result;
  }

  // Check we can load the library.
  if (!($library = libraries_load('jsmin-php')) || empty($library['loaded'])) {
    $result = FALSE;
    return $result;
  }

  // Try to compile the file locally.
  $test_string = 'var i = 4; alert(i+i);';
  $compiled_string = JSMin::minify($test_string);

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
function jsmin_php_local_available() {
  return jsmin_php_compiler_exists() && jsmin_php_local_works();
}

/**
 * Tests whether the local compiler is available.
 */
function jsmin_php_compiler_exists() {
  $library = libraries_detect('jsmin-php');
  return !empty($library['installed']);
}
