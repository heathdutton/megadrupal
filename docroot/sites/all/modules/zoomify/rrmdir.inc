<?php

/**
 * @file
 * Adds function rrmdir(): recursive rmdir
 * from http://lixlpixel.org/recursive_function/php/recursive_directory_delete/
 */
if (!function_exists('rrmdir')) {
  function rrmdir($directory, $empty = FALSE) {
    if (substr($directory, -1) == '/') {
      $directory = substr($directory, 0, -1);
    }
    if (!file_exists($directory) || !is_dir($directory)) {
      return FALSE;
    }
    elseif (is_readable($directory)) {
      $handle = opendir($directory);
      while (FALSE !== ($item = readdir($handle))) {
        if ($item != '.' && $item != '..') {
          $path = $directory . '/' . $item;
          if (is_dir($path)) {
            rrmdir($path);
          }
          else {
            unlink($path);
          }
        }
      }
      closedir($handle);
      if (!$empty) {
        if (!rmdir($directory)) {
          return FALSE;
        }
      }
    }
    return TRUE;
  }
}
