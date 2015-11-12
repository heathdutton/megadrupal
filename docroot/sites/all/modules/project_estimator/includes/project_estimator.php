<?php

/**
 * @file
 * This is the class that containin all the function which process the
 * statistics.
 */

define('PROJECT_ESTIMATOR_MAX_DEPTH', 100);

class ProjectEstimator {

  public $root = '';
  public $filters = array();
  public $checkSubdirs = TRUE;
  public $files = array();
  public $totalFiles = 0;
  public $totalLines = 0;
  public $totalChars = 0;
  public $totalSize = 0;

  /**
   * function to get report info
   */
  function getInfo() {
    if (!$this->processDir()) {
      return FALSE;
    }

    return TRUE;
  }

  /**
   * function to check file
   */
  function checkFile($fname) {
    if (!sizeof($this->filters)) {
      return TRUE;
    }
    $matched = FALSE;
    foreach ($this->filters as $filter) {
      if (preg_match($filter, $fname)) {
        $matched = TRUE;
      }
    }
    return $matched;
  }

  /**
   * function to process directories
   */
  function processDir($dir_name = '', $depth = 0) {
    if ($depth > PROJECT_ESTIMATOR_MAX_DEPTH) {
      trigger_error('Maximum depth is reached in ' . $dir_name . '.', E_USER_ERROR);
      return FALSE;
    }
    if (empty($dir_name)) {
      $dir_name = $this->root;
    }
    if (empty($dir_name)) {
      trigger_error('No root directory specified.', E_USER_ERROR);
      return FALSE;
    }
    $dir_name = str_replace('\\', '/', $dir_name);
    if (drupal_substr($dir_name, -1) != '/') {
      $dir_name .= '/';
    }
    if (!is_dir($dir_name)) {
      trigger_error('Cannot find ' . $dir_name . ' directory.', E_USER_ERROR);
      return FALSE;
    }
    if (!is_readable($dir_name)) {
      trigger_error('The directory ' . $dir_name . ' is not readable.', E_USER_ERROR);
      return FALSE;
    }
    if (!($dir_handle = opendir($dir_name))) {
      trigger_error('The directory ' . $dir_name . ' cannot be opened.', E_USER_ERROR);
      return FALSE;
    }
    $this->files = array();
    $subdirs = array();
    while (FALSE !== ($fname = readdir($dir_handle))) {
      if ($fname == '.' || $fname == '..') {
        continue;
      }
      $fname = $dir_name . $fname;
      if (is_dir($fname) && !$this->checkSubdirs) {
        continue;
      }
      if (is_dir($fname)) {
        $subdirs[] = $fname;
      }
      else {
        if ($this->checkFile($fname)) {
          $finfo = $this->getFileInfo($fname);
          $this->files[] = $finfo;
          $this->totalLines += $finfo['num_lines'];
          $this->totalChars += $finfo['num_chars'];
          $this->totalSize += $finfo['file_size'];
          $this->totalFiles++;
        }
      }
    }
    closedir($dir_handle);
    foreach ($subdirs as $subdir) {
      $this->files = array_merge($this->files, $this->processDir($subdir, $depth + 1));
    }
    return $this->files;
  }

  /**
   * function to get file info
   */
  function getFileInfo($fname) {
    if (!$lines = file($fname)) {
      trigger_error('Cannot read the file ' . $fname . '.', E_USER_ERROR);
      return FALSE;
    }
    $contents = implode('', $lines);
    $contents = str_replace(' ', '', $contents);
    $contents = str_replace("\n", '', $contents);
    $contents = str_replace("\r", '', $contents);
    $contents = str_replace("\t", '', $contents);
    $info['file_name'] = $fname;
    $info['file_size'] = filesize($fname);
    $info['num_lines'] = sizeof($lines);
    $info['num_chars'] = drupal_strlen($contents);
    return $info;
  }

}
