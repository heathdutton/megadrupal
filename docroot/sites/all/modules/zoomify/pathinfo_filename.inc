<?php

/**
 * @file
 * Adds function pathinfo_filename() for older PHP versions
 * Taken from http://www.php.net/manual/en/function.pathinfo.php#77748
 */
if (!function_exists('pathinfo_filename')) {
  if (version_compare(phpversion(), "5.2.0", "<")) {
    function pathinfo_filename($path) {
      $temp = pathinfo($path);
      if ($temp['extension']) {
        $temp['filename'] = substr($temp['basename'], 0, strlen($temp['basename']) -strlen($temp['extension']) -1);
      }
      else {
        $temp['filename'] = $temp['basename'];
      }
      return $temp;
    }
  }
  else {
    function pathinfo_filename($path) {
      return pathinfo($path);
    }
  }
}
