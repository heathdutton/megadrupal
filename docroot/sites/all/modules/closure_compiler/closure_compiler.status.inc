<?php

/**
 * @file
 * Contains status-related functions.
 */

/**
 * Displays a somewhat more in-depth status page.
 */
function closure_compiler_status() {
  module_load_include('inc', 'closure_compiler', 'closure_compiler.functions');
  $js_files = closure_compiler_get_js_files();

  // Output list of unoptimized files.
  $output = '<h2>Unoptimized JS Files</h2>';
  $unoptimized = array();
  foreach ($js_files['unoptimized'] as $uri => $js_file) {
    $unoptimized[] = array(
      $uri,
      format_date($js_file['mod_time'], 'short'),
      _closure_compiler_sort_filesize($js_file['js_size']),
      $js_file['gz_size'] ? _closure_compiler_sort_filesize($js_file['gz_size']) . _closure_compiler_js_percent($js_file['gz_size'], $js_file['js_size']) : '',
    );
  }
  if (empty($unoptimized)) {
    $output .= '<p>No unoptimized files found</p>';
  }
  else {
    $output .= theme('table', array(
      'header' => array(
        'File URI',
        'Modification time',
        'JS Size',
        'GZ Size (% of orig JS)',
      ),
      'rows' => $unoptimized,
    ));
  }

  // Output list of optimized files.
  $output .= '<h2>Optimized JS Files</h2>';
  $optimized = array();
  foreach ($js_files['optimized'] as $uri => $js_file) {
    $optimized[] = array(
      $uri,
      format_date($js_file['mod_time'], 'short'),
      _closure_compiler_sort_filesize($js_file['js_size']) . _closure_compiler_js_percent($js_file['js_size'], $js_file['orig_js_size']),
      $js_file['gz_size'] ? _closure_compiler_sort_filesize($js_file['gz_size']) . _closure_compiler_js_percent($js_file['gz_size'], $js_file['orig_js_size']) : '',
    );
  }
  if (empty($optimized)) {
    $output .= '<p>No optimized files found.</p>';
  }
  else {
    $output .= theme('table', array(
      'header' => array(
        'File URI',
        'Modification time',
        'JS Size (% of orig JS)',
        'GZ Size (% of orig JS)',
      ),
      'rows' => $optimized,
    ));
  }

  return $output;
}

/**
 * Converts a file size into a shorter SI version.
 *
 * http://stackoverflow.com/questions/2510434/
 *  php-format-bytes-to-kilobytes-megabytes-gigabytes
 */
function _closure_compiler_sort_filesize($size) {
  $units = array('B', 'KB', 'MB');
  $size = max($size, 0);
  $pow = floor(($size ? log($size) : 0) / log(1024));
  // $pow = min($pow, count($units) - 1);
  $pow = min($pow, 2);
  $size /= pow(1024, $pow);
  return number_format($size, $pow ? 2 : 0) . ' ' . $units[$pow];
}

/**
 * Calculates the percentage size compared to the original.
 */
function _closure_compiler_js_percent($new_size, $original_size) {
  return ' (' . number_format($new_size / $original_size * 100, 1) . '%)';
}
