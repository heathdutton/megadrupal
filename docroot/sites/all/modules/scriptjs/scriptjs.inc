<?php

/**
 * @file
 * Helper functions for Script.js. 
 */

/**
 * Get the path of to the selected $script.js library file.
 * 
 * @return string
 */
function scriptjs_get_path() {
  static $path;

  if (!$path) {
    // Decide which version of $script.js to use, minified (production) or normal (development).
    $scriptjs_version = (variable_get('scriptjs_min', TRUE)) ? 'dist/script.min.js' : 'dist/script.js';
    $path = scriptjs_path() . DIRECTORY_SEPARATOR . $scriptjs_version;
  }

  return $path;
}

/**
 * Get the path to $script.js library.
 * Uses Libraries API if available.
 *
 * @param bool
 * @return string
 */
function scriptjs_path($refresh = FALSE) {
  static $path;

  if ($refresh === TRUE || !isset($path)) {
    $path = function_exists('libraries_get_path') ? libraries_get_path('scriptjs') : 'sites/all/libraries/scriptjs';
  }

  return $path;
}

/**
 * Determines whether to use $script.js at the specified page.
 */
function _scriptjs_is_enabled() {
  if (variable_get('scriptjs_enable', FALSE) && !defined('MAINTENANCE_MODE')) {
    // Include both the path alias and normal path for matching.
    $current_path = drupal_get_path_alias($_GET['q']);
    // Bypass administration pages when option disabled.
    $disable_admin = variable_get('scriptjs_disable_admin', FALSE);

    if ($disable_admin && strpos($current_path, 'admin') === 0) {
      return FALSE;
    }
    else if ($current_path != $_GET['q']) {
      $current_path = (array) $current_path;
      $current_path[] = $_GET['q'];
    }
    $paths = variable_get('scriptjs_exclude', '');
    return !drupal_match_path($current_path, $paths);
  }
  return FALSE;
}

/**
 * Array filter callback.
 * Filter scripts with defer flag.
 * 
 * @see scriptjs_js_alter().
 * @see array_filter().
 */
function _scriptjs_filter_defer($value) {
  return (is_array($value) && isset($value['defer']) && $value['defer']);
}

/**
 * Array filter callback.
 * Filter scripts in header scope.
 * 
 * @see scriptjs_js_alter().
 * @see array_filter().
 */
function _scriptjs_filter_header($value) {
  return (is_array($value) && isset($value['scope']) && $value['scope'] === 'header' && !_scriptjs_filter_defer($value));
}

/**
 * Array filter callback.
 * Filter scripts in footer scope.
 * Since themes can define their own regions we assume that everything not in header is in footer.
 * 
 * @see scriptjs_js_alter().
 * @see array_filter().
 */
function _scriptjs_filter_footer($value) {
  return (is_array($value) && isset($value['scope']) && $value['scope'] !== 'header' && !_scriptjs_filter_defer($value));
}

/**
 * Array filter callback.
 * Filter inline scripts.
 * 
 * @see scriptjs_js_alter().
 * @see array_filter().
 */
function _scriptjs_filter_inline($value) {
  return (is_array($value) && isset($value['type']) && $value['type'] === 'inline');
}

/**
 * Array map callback.
 * Flatten items from $javascript array.
 * 
 * @see scriptjs_js_alter().
 * @see array_map().
 */
function _scriptjs_flatten($item, $default_query_string) {
  // Add version if available.
  if (isset($item['version'])) {
    $fn = $item['data'];
    $qs = "v={$item['version']}";
  }
  // Replace drupal.js with our modified version.
  else if ($item['data'] === 'misc/drupal.js') {
    $fn = drupal_get_path('module', 'scriptjs') . DIRECTORY_SEPARATOR . 'replace' . DIRECTORY_SEPARATOR . 'drupal.js';
    $qs = 'v=1.0';
  }
  // Add default query string if preprocessable or timestamp if not.
  else {
    $fn = $item['data'];
    $qs = $item['preprocess'] ? $default_query_string : time();
  }

  return file_create_url($fn) . "?{$qs}";
}