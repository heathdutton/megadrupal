<?php

/**
 * @file
 * Contains the actual compilation functions.
 */

/**
 * Adds compilation parameters for Google's Closure Compiler service.
 */
function _google_closure_compiler_compile_params() {
  // Perform local compiling check.
  module_load_include('inc', 'google_closure_compiler', 'google_closure_compiler.checks');
  google_closure_compiler_confirm_local();

  module_load_include('inc', 'google_closure_compiler', 'google_closure_compiler.defines');
  module_load_include('inc', 'closure_compiler', 'closure_compiler.defines');
  return array(
    'compilation_level' => variable_get('google_closure_compiler_compilation_level', GOOGLE_CLOSURE_COMPILER_SIMPLE),
    'process_method' => variable_get('google_closure_compiler_process_method', CLOSURE_COMPILER_SEND_CONTENTS),
  );
}

/**
 * Compiles a single JS file.
 *
 * @param string $uri
 *   The location of the JS file to compile.
 * @param array $params
 *   An associative array containing two elements:
 *   'compilation_level' - the compilation level to use.
 *   'process_method' - the process method to use.
 *   See (google_)closure_compiler.defines.inc.
 *
 * @return string
 *   The compilation result (FALSE or empty string on failure).
 */
function _google_closure_compiler_compile_file($uri, $params) {
  module_load_include('inc', 'closure_compiler', 'closure_compiler.defines');

  // Defaults, in case $params is incorrect.
  if (!is_array($params)) {
    $params = array();
  }
  if (!isset($params['process_method']) || !in_array($params['process_method'], array(CLOSURE_COMPILER_LOCAL, CLOSURE_COMPILER_SEND_CONTENTS, CLOSURE_COMPILER_SEND_URL))) {
    $params['process_method'] = variable_get('google_closure_compiler_process_method', CLOSURE_COMPILER_SEND_CONTENTS);
  }
  if (!isset($params['compilation_level']) || !in_array($params['compilation_level'], array(GOOGLE_CLOSURE_COMPILER_WHITESPACE, GOOGLE_CLOSURE_COMPILER_SIMPLE, GOOGLE_CLOSURE_COMPILER_ADVANCED))) {
    $params['compilation_level'] = variable_get('google_closure_compiler_compilation_level', GOOGLE_CLOSURE_COMPILER_SIMPLE);
  }

  // Run the compilation, depending on which method is selected.
  if (CLOSURE_COMPILER_LOCAL == $params['process_method']) {
    return _google_closure_compiler_compile_file_local($uri, $params['compilation_level']);
  }
  else {
    if (CLOSURE_COMPILER_SEND_CONTENTS == $params['process_method'] && filesize($uri) <= 200000) {
      $params['js_code'] = file_get_contents($uri);
    }
    else {
      $params['code_url'] = file_create_url($uri);
    }
    unset($params['process_method']);
    return _google_closure_compiler_compile_file_api($params);
  }
}

/**
 * Compiles a JS file locally.
 *
 * @param string $uri
 *   The location of the JS file to compile.
 * @param string $compilation_level
 *   The compilation level to use (available options are defined).
 *
 * @return string
 *   The compilation result (FALSE or empty string on failure).
 */
function _google_closure_compiler_compile_file_local($uri, $compilation_level) {
  module_load_include('inc', 'google_closure_compiler', 'google_closure_compiler.defines');

  // The compiler script writes to a temporary file, prepends the signature
  // and replaces the original javascript file - the raw console output
  // can include error or warning messages, which breaks JS.
  $temp_file_path = drupal_tempnam(file_directory_temp(), 'jscc');
  $compiler_path = drupal_get_path('module', 'google_closure_compiler') . '/' . GOOGLE_CLOSURE_COMPILER_FILE;
  $command = 'java -jar ' . $compiler_path . ' --compilation_level ' . $compilation_level . ' --js "' . drupal_realpath(check_plain($uri)) . '" --js_output_file ' . $temp_file_path;
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

/**
 * Compiles a JS file using the API.
 *
 * @param array $params
 *   Containing 'code_url' or 'js_code', and 'compilation_level'.
 *
 * @return string
 *   The compilation result (FALSE or empty string on failure).
 */
function _google_closure_compiler_compile_file_api($params) {
  module_load_include('inc', 'google_closure_compiler', 'google_closure_compiler.defines');

  // Perform the API request.
  $params['output_info'] = 'compiled_code';
  $params['output_format'] = 'json';
  $response = drupal_http_request(GOOGLE_CLOSURE_COMPILER_URL, array(
    'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
    'method' => 'POST',
    'data' => http_build_query($params, '', '&'),
  ));

  // Check the response is valid.
  if (200 != $response->code || !$response->data) {
    watchdog('closure_compiler', 'Request error: @request', array('@request' => print_r($response, TRUE)));
    return FALSE;
  }

  // Decode json in the response.
  $compiled_code = json_decode($response->data, TRUE);
  if (!$compiled_code || !is_array($compiled_code)) {
    watchdog('closure_compiler', 'Failed to parse JSON response: @response', array('@response' => $response->data));
    return FALSE;
  }

  // Check for server errors.
  if (array_key_exists('serverErrors', $compiled_code)) {
    watchdog('closure_compiler', 'Server error: @error', array('@error' => print_r($compiled_code['serverErrors'], TRUE)));
    return FALSE;
  }

  // Finally check the actual compiled code!
  if (array_key_exists('compiledCode', $compiled_code) && !empty($compiled_code['compiledCode'])) {
    return $compiled_code['compiledCode'];
  }
  // The code is not present, or is empty.
  else {
    return FALSE;
  }
}
