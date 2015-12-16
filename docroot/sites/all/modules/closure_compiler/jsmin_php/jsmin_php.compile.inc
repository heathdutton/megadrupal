<?php

/**
 * @file
 * Contains the actual compilation functions.
 */

/**
 * Adds compilation parameters for the JSMin-PHP script.
 */
function _jsmin_php_compile_params() {
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
function _jsmin_php_compile_file($uri, $params) {
  // Run the compilation.
  return _jsmin_php_compile_file_local($uri);
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
function _jsmin_php_compile_file_local($uri) {
  // Check we can load the library.
  if (!($library = libraries_load('jsmin-php')) || empty($library['loaded'])) {
    $result = FALSE;
    return $result;
  }

  // The compiler script reads the JS, "compiles" it, and returns the result.
  $compiled_js = JSMin::minify(file_get_contents($uri));

  // Check whether the compiled string contains any content.
  if (!$compiled_js) {
    return FALSE;
  }

  return $compiled_js;
}
