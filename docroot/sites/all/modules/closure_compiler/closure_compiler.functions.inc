<?php

/**
 * @file
 * Contains useful functions to be used elsewhere.
 */

/**
 * Returns an array containing all of the JS file URIs and modification times.
 */
function closure_compiler_get_js_files() {
  module_load_include('inc', 'closure_compiler', 'closure_compiler.defines');

  $js_files = file_scan_directory(CLOSURE_COMPILER_JS_PATH, '/\.js$/', array('recurse' => FALSE, 'nomask' => '/\.src\.js$/'));

  $optimized = array();
  $unoptimized = array();
  foreach ($js_files as $uri => $js_file) {
    if (closure_compiler_is_file_optimized($uri, $js_file)) {
      $optimized[$uri] = array(
        'js_size' => filesize($uri),
        'mod_time' => filemtime($uri),
        'gz_size' => file_exists($uri . '.gz') ? filesize($uri . '.gz') : FALSE,
      );
      $source_uri = closure_compiler_get_source_uri($uri);
      if (file_exists($source_uri)) {
        $optimized[$uri] += array(
          'orig_js_size' => filesize($source_uri),
          'orig_mod_time' => filemtime($source_uri),
        );
      }
    }
    else {
      $unoptimized[$uri] = array(
        'js_size' => filesize($uri),
        'mod_time' => filemtime($uri),
        'gz_size' => file_exists($uri . '.gz') ? filesize($uri . '.gz') : FALSE,
      );
    }
  }

  return array(
    'optimized' => $optimized,
    'unoptimized' => $unoptimized,
  );
}

/**
 * Check if a JS file has already been optimized.
 *
 * The URI is not processed, ensure it is the same each time!
 *
 * @param string $uri
 *   A valid path to a file.
 *
 * @return bool
 *   Whether the JS file is optimized.
 */
function closure_compiler_is_file_optimized($uri) {
  // If the file begins with a link to its source, then it has been optimized.
  if (CLOSURE_COMPILER_SOURCE_START == file_get_contents($uri, NULL, NULL, 0, strlen(CLOSURE_COMPILER_SOURCE_START))) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Returns the URI to a source JS file, given a URI.
 */
function closure_compiler_get_source_uri($uri) {
  return str_replace('.js', '.src.js', $uri);
}

/**
 * Returns the available services.
 *
 * Each service should return an associative array containing a single element:
 * 'module_name' => t('Friendly Name').
 *
 * @param $update
 *   Optional parameter, if set to true the hook will be invoked to refresh
 *   the service list, otherwise the variable will be used.
 */
function closure_compiler_get_services($update = FALSE) {
  if ($update) {
    $services = module_invoke_all('closure_compiler_service');
    variable_set('closure_compiler_available_services', $services);
    return $services;
  }
  else {
    return variable_get('closure_compiler_available_services', array());
  }
}