<?php

/**
 * @file
 * Contains the actual compilation functions.
 */

/**
 * Adds compilation parameters for the Uglify2 service.
 */
function _uglify2_compile_params() {
  module_load_include('inc', 'closure_compiler', 'closure_compiler.defines');
  return array(
    'process_method' => variable_get('uglify2_process_method', CLOSURE_COMPILER_SEND_CONTENTS),
  );
}

/**
 * Compiles a single JS file.
 *
 * @param string $uri
 *   The location of the JS file to compile.
 * @param array $params
 *   An associative array containing one element:
 *   'process_method' - the process method to use.
 *   See closure_compiler.defines.inc.
 *
 * @return string
 *   The compilation result (FALSE or empty string on failure).
 */
function _uglify2_compile_file($uri, $params) {
  module_load_include('inc', 'closure_compiler', 'closure_compiler.defines');

  // Defaults, in case $params is incorrect.
  if (!is_array($params)) {
    $params = array();
  }
  if (!isset($params['process_method']) || !in_array($params['process_method'], array(CLOSURE_COMPILER_SEND_CONTENTS, CLOSURE_COMPILER_SEND_URL))) {
    $params['process_method'] = variable_get('uglify2_process_method', CLOSURE_COMPILER_SEND_CONTENTS);
  }

  // Run the compilation, depending on which method is selected.
  if (CLOSURE_COMPILER_SEND_CONTENTS == $params['process_method'] && filesize($uri) <= 200000) {
    $params['js_code'] = file_get_contents($uri);
  }
  else {
    $params['code_url'] = file_create_url($uri);
  }
  return _uglify2_compile_file_api($params);
}

/**
 * Compiles a JS file using the API.
 *
 * @param array $params
 *   Containing 'code_url' or 'js_code'.
 *
 * @return string
 *   The compilation result (FALSE or empty string on failure).
 */
function _uglify2_compile_file_api($params) {
  module_load_include('inc', 'uglify2', 'uglify2.defines');

  // Perform the API request.
  $response = drupal_http_request(UGLIFY2_URL, array(
    'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
    'method' => 'POST',
    'data' => http_build_query($params, '', '&'),
  ));

  // Check the response is valid.
  if (200 != $response->code || !$response->data) {
    watchdog('uglify2', 'Request error: @request', array('@request' => print_r($response, TRUE)));
    return FALSE;
  }

  // Check for server errors.
  if (isset($response->error)) {
    watchdog('uglify2', 'Server error: @error', array('@error' => print_r($response->data, TRUE)));
    return FALSE;
  }

  // Finally check the returned code - if the API was unable to retrieve the
  // JS file, it instead returns the HTML page - we don't want to save this
  // in place of our JS!
  if (FALSE === strpos($response->data, '<title>UglifyJS JavaScript minification</title>')) {
    return $response->data;
  }
  else {
    return FALSE;
  }
}
