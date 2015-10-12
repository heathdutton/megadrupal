<?php

/**
 * @file
 * Constants and utility functions for the Grammar Parser UI module.
 *
 * Copyright 2009-10 by Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

/**
 * The root installation directory.
 */
if (!defined('DRUPAL_ROOT')) {
  define('DRUPAL_ROOT', getcwd());
}

/**
 * The default directory to store parsed source code files.
 * Relative to file_directory_path().
 *
 * @var string
 */
define('PARSER_DIR', 'gpui');

/**
 * The administration menu path based on Drupal version.
 *
 * @var string
 */
define('PARSER_MENU_PATH', gpui_parser_menu_path());

/**
 * Returns the administration menu path based on Drupal version.
 */
function gpui_parser_menu_path() {
  if (defined('VERSION')) {
    if (substr(VERSION, 0, 1) == 6) {
      return 'admin/build/gpui';
    }
    else {
      return 'admin/config/development/gpui';
    }
  }
}

/**
 * Removes all files from a directory and optionally removes the directory.
 *
 * @param string $path
 *   Directory path.
 * @param boolean $remove_me
 *   If TRUE, then remove the directory as well as its files.
 */
function gpui_clean_directory($path, $remove_me = FALSE) {
  $path = $path . '/';
  if (!is_dir($path)) {
    return;
  }
  $files = scandir($path);
  foreach ($files as $file) {
    if ($file != '.' && $file != '..') {
      $file_path = $path . $file;
      if (is_dir($file_path)) {
        gpui_clean_directory($file_path, TRUE);
      }
      else {
        unlink($file_path);
      }
    }
  }
  if ($remove_me) {
    rmdir($path);
  }
}

/**
 * Prints memory usage to log.
 *
 * @param string $step
 *   A string describing the code step when the memory usage is gathered.
 */
function gpui_log_memory_use($step) {
  static $path = '';

  if (!$path) {
    $path = gpui_path('memory');
    // Clearing here works except with bulk which spawns processes.
//     gpui_path_clear('memory');
  }
  gpui_path_print($path, "\n\n\n====================\n$step");
  gpui_path_print($path, 'Peak: ' . number_format(memory_get_peak_usage(TRUE), 0, '.', ',') . ' bytes');
  gpui_path_print($path, 'Curr: ' . number_format(memory_get_usage(TRUE), 0, '.', ',') . ' bytes');
}

/**
 * Prints log information if log flag is on.
 *
 * @param mixed $text
 *   A string, array, or object to print.
 */
function gpui_log_print($text) {
  global $_gpui_log;
  static $path = '';

  if (!$_gpui_log) {
    return;
  }
  if (!$path) {
    $path = gpui_path('log');
//     gpui_path_clear('log');
  }
  gpui_path_print($path, $text);
}

/**
 * Prints debug information if debug flag is on.
 *
 * @param mixed $text
 *   A string, array, or object to print.
 */
function gpui_debug_print($text) {
  global $_gpui_debug;
  static $path = '';

  if (!$_gpui_debug) {
    return;
  }
  if (!$path) {
    $path = gpui_path('debug');
//     gpui_path_clear();
  }
  gpui_path_print($path, $text);
}

/**
 * Clears the contents of a file.
 *
 * @param string $type
 *   File type.
 */
function gpui_path_clear($type = 'debug') {
  file_put_contents(gpui_path($type), '');
}

/**
 * Prints information to a file.
 *
 * @param string $path
 *   File path.
 * @param mixed $text
 *   A string, array, or object to print.
 */
function gpui_path_print($path, $text) {
  if ($text instanceof PGPList) {
    file_put_contents($path, $text->print_r(), FILE_APPEND);
  }
  elseif ($text instanceof PGPBase) {
    file_put_contents($path, $text->print_r(), FILE_APPEND);
  }
  elseif ($text instanceof stdClass) {
    file_put_contents($path, print_r($text, 1), FILE_APPEND);
  }
  elseif (is_object($text)) {
    file_put_contents($path, "Unknown object class\n", FILE_APPEND);
  }
  elseif (is_array($text)) {
    file_put_contents($path, print_r($text, 1), FILE_APPEND);
  }
  else {
    file_put_contents($path, $text . "\n", FILE_APPEND);
  }
}

function plp($text) {
  gpui_log_print($text);
}

function pdp($text, $description = '') {
  if ($description) {
    $description .= ' ==>'; // Use two '=' so easier to find in file.
    gpui_debug_print($description);
  }
  gpui_debug_print($text);
}

/**
 * Returns path to file or files directory.
 *
 * @param string $type
 *   Type of file to return path to. If blank, return directory path.
 * @return string
 *   Path to file or directory.
 */
function gpui_path($type = '') {
  static $path = '';

  if (!$path) {
    // @todo In D7, to run the parser from a script, we can not call
    // gpui_directory_path() because file_directory_path() may not be defined.
    $path = gpui_dir(); // $path = gpui_directory_path('parser', FALSE);
  }
  return $type ? $path . '/' . $type . '.txt' : $path;
}

/**
 * Returns path to parser directory.
 *
 * @param string $filename
 *   String of the patch filename.
 * @return string
 *   Path to file.
 */
function gpui_dir() {
  static $path = '';

  if (!$path) {
    $path = '.';
    if (function_exists('file_directory_path')) {
      $path = file_directory_path();
      if (is_dir($path . '/' . variable_get('gpui_dir', PARSER_DIR))) {
        $path .= '/' . variable_get('gpui_dir', PARSER_DIR);
      }
    }
  }
  return $path;
}

/**
 * Returns full directory path relative to sites directory.
 *
 * @param string $name
 *   Name of the directory.
 * @param boolean $add_slash
 *   Indicates whether to add a trailing slash.
 * @param boolean $stream_format
 *   Indicates whether to use the actual path or a stream protocol.
 * @return string
 *   A string of the directory path.
 */
function gpui_directory_path($name, $add_slash = TRUE, $stream_format = FALSE) {
  $slash = $add_slash ? '/' : '';
  $prefix_no_slash = $stream_format ? file_default_scheme() . ':/' : file_directory_path();
  $prefix = $prefix_no_slash . '/';

  switch ($name) {
    case 'parser':
      return $prefix . variable_get('gpui_dir', PARSER_DIR) . $slash;
    case '':
      return $prefix_no_slash; // @todo Is this correct with a stream format?
    default:
      return $prefix . $name . $slash;
  }
}

/**
 * Returns the local public directory path.
 *
 * Adapted from function removed from core on 2010-09-01
 * (see http://drupal.org/cvs?commit=415020).
 *
 * @return string
 *   A string containing the directory path of a stream. FALSE is returned if
 *   the scheme is invalid or a wrapper could not be instantiated.
 */
if (!function_exists('file_directory_path') && function_exists('file_stream_wrapper_get_instance_by_scheme')) {
  function file_directory_path($scheme = 'public') {
    if ($wrapper = file_stream_wrapper_get_instance_by_scheme($scheme)) {
      return $wrapper->getDirectoryPath();
    }
    else {
      return FALSE;
    }
  }
}
