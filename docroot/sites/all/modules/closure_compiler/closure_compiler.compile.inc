<?php

/**
 * @file
 * Contains the actual compilation functions.
 */

/**
 * Main compilation function, run from the cron function.
 */
function closure_compiler_compile() {
  module_load_include('inc', 'closure_compiler', 'closure_compiler.defines');

  // Ensure the directory exists and is writeable.
  $js_dir = CLOSURE_COMPILER_JS_PATH;
  if (!file_prepare_directory($js_dir, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
    watchdog('closure_compiler', '%path directory not available', array('%path' => CLOSURE_COMPILER_JS_PATH), WATCHDOG_WARNING);
    return;
  }

  // Create list of files to compile.
  module_load_include('inc', 'closure_compiler', 'closure_compiler.functions');
  $js_files = closure_compiler_get_js_files();
  if (empty($js_files['unoptimized'])) {
    if (empty($js_files['optimized'])) {
      watchdog('closure_compiler', 'No JavaScript files found at %path', array('%path' => CLOSURE_COMPILER_JS_PATH), WATCHDOG_NOTICE);
    }
    else {
      watchdog('closure_compiler', 'No unoptimized JavaScript files found at %path', array('%path' => CLOSURE_COMPILER_JS_PATH), WATCHDOG_INFO);
    }
    return;
  }

  module_load_include('inc', 'closure_compiler', 'closure_compiler.functions');

  // Proceed to optimize the JS files.
  $process_limit = variable_get('closure_compiler_process_limit', CLOSURE_COMPILER_DEFAULT_LIMIT);
  $enabled_services = variable_get('closure_compiler_service', array());
  $compilation_params = array();

  // Allow sub-modules to add additional parameters.
  foreach ($enabled_services as $module) {
    if (module_load_include('inc', $module, $module . '.compile')) {
      $function = '_' . $module . '_compile_params';
      if (function_exists($function)) {
        $compilation_params[$module] = call_user_func($function);
      }
      else {
        $compilation_params[$module] = array();
      }
    }
  }

  $succeeded = $failed = 0;
  foreach ($js_files['unoptimized'] as $uri => $js_file) {
    // Stop processing if limit is reached.
    if ($process_limit && ($succeeded + $failed) == $process_limit) {
      break;
    }

    // Attempt to optimize an individual file.
    $source_uri = closure_compiler_get_source_uri($uri);
    if (!file_unmanaged_copy($uri, $source_uri, FILE_EXISTS_REPLACE) || !closure_compiler_compile_file($uri, $source_uri, $compilation_params)) {
      ++$failed;
    }
    else {
      ++$succeeded;
    }
  }

  if (($succeeded + $failed) > 0) {
    watchdog('closure_compiler', 'Successfully compiled @succeeded javascript files, @failed failed.', array('@succeeded' => $succeeded, '@failed' => $failed), WATCHDOG_INFO);
  }
}

/**
 * Compiles a single JS file.
 *
 * @param string $uri
 *   The location of the JS file to compile.
 * @param string $source_uri
 *   The location to which the source link should point. If the source should
 *   not be available, then pass the boolean value FALSE.
 * @param array compilation_params
 *   An associative array, containing 'module' => array(parameters).
 *
 * @return bool
 *   Whether the compilation was successful.
 */
function closure_compiler_compile_file($uri, $source_uri, $compilation_params) {
  module_load_include('inc', 'closure_compiler', 'closure_compiler.defines');

  $result = FALSE;
  foreach ($compilation_params as $module => $params) {
    if (module_load_include('inc', $module, $module . '.compile')) {
      $function = '_' . $module . '_compile_file';
      if (function_exists($function)) {
        $sub_result = call_user_func($function, $uri, $params);
        if ($sub_result && ($result === FALSE || strlen($sub_result) < strlen($result))) {
          $result = $sub_result;
        }
      }
    }
  }

  if ($result) {
    // Overwrite the original file.
    $source_url = $source_uri ? file_create_url($source_uri) : 'Unavailable';
    if (!file_unmanaged_save_data(CLOSURE_COMPILER_SOURCE_START . $source_url . CLOSURE_COMPILER_SOURCE_END . $result, $uri, FILE_EXISTS_REPLACE)) {
      return FALSE;
    }

    // Replace GZip-compressed file if necessary.
    if (variable_get('js_gzip_compression', TRUE) && variable_get('clean_url', 0) && extension_loaded('zlib')) {
      file_unmanaged_save_data(gzencode(file_get_contents($uri), 9, FORCE_GZIP), $uri . '.gz', FILE_EXISTS_REPLACE);
    }
    elseif (file_exists($uri . '.gz')) {
      file_unmanaged_delete($uri . '.gz');
    }

    return TRUE;
  }

  return FALSE;
}
