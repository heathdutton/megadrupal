<?php
/**
 * @file
 * include.settings.inc
 *
 * This settings file should be included from settings.php file.
 * Put all include files related logic here.
 *
 * Everything in this file is executed on DRUPAL_BOOTSTRAP_CONFIGURATION
 * bootstrap phase, so don't put here nothing not available on this phase.
 */

define('INSTANCE_SETTINGS_FILE_EXTENSION', '.settings.inc');

// Included part (no function).
// $dirs, $params variables should be defined in global;
$includes = array();
foreach ($settings_sources as &$dir) {
  if (isset($dir['pattern_types'])) {
    $dir['patterns'] = array();
    foreach ($dir['pattern_types'] as $pattern_type) {
      $dir['patterns'] += $pattern_types[$pattern_type];
    }
    instance_settings_include_get_includes($dir['dir'], $dir['patterns'], $params, $includes);
  }
}

$conf['instance_settings_included_files'] = array();
// Include files from provided array.
foreach ($includes as $filename) {
  if (file_exists($filename)) {
    $tempconf = $conf;
    require $filename;
    $conf['instance_settings_included_files'][] = $filename;
    foreach ($conf as $key => $value) {
      if (is_array($value) && isset($tempconf[$key])) {
        $conf[$key] = instance_settings_include_array_merge_deep_no_duplicate_values(array($tempconf[$key], $conf[$key]));
      }
    }
    $conf = array_merge($tempconf, $conf);
  }
}

function instance_settings_get_patterns($params) {
  $patterns = array();
  foreach ($params as $param) {
    $patterns += $param;
  }
}

function instance_settings_include_get_includes($dir, $patterns, $params, &$includes) {
  foreach ($patterns as $pattern) {
    $includes[] = $dir . '/' . format_string($pattern, $params) . INSTANCE_SETTINGS_FILE_EXTENSION;
  }

  return $includes;
}

function instance_settings_include_array_merge_deep_no_duplicate_values($arrays) {
  $result = array();

  foreach ($arrays as $array) {
    foreach ($array as $key => $value) {
      // Renumber integer keys as array_merge_recursive() does. Note that PHP
      // automatically converts array keys that are integer strings (e.g., '1')
      // to integers.
      if (is_integer($key)) {
        if (is_array($value)) {
          $result[] = $value;
        }
        elseif (!in_array($value, $result)) {
          $result[] = $value;
        }
      }
      // Recurse when both values are arrays.
      elseif (isset($result[$key]) && is_array($result[$key]) && is_array($value)) {
        $result[$key] = instance_settings_include_array_merge_deep_no_duplicate_values(array($result[$key], $value));
      }
      // Otherwise, use the latter value, overriding any previous value.
      else {
        $result[$key] = $value;
      }
    }
  }

  return $result;
}

