<?php

/**
 * @file
 * Custom CSS-Crush interface for writing files,
 * retrieving files and checking caches.
 */
class CssCrushDrupalIo extends CssCrush\IO {

  /**
   * Overrides CssCrush\IO::getOutputDir().
   */
  public function getOutputDir() {

    return DRUPAL_ROOT . '/' . CSSCRUSH_OUT_DIR;
  }

  /**
   * Overrides CssCrush\IO::getOutputFileName().
   */
  public function getOutputFileName() {

    $process = $this->process;
    $options = $process->options;
    $input = $process->input;

    if (!empty($options->output_file)) {
      $output_basename = basename($options->output_file, '.css');
    }

    // Get a hashed fingerprint of the file (based on path).
    // MD5 is not used for security reasons but as a fast way for creating a
    // unique filename for safely saving all files to a single directory.
    $fingerprint = 'null';
    if (!empty($input->path) && strlen($input->path) > strlen(DRUPAL_ROOT)) {
      $path = trim(substr($input->path, strlen(DRUPAL_ROOT)), '\\/');
      $fingerprint = md5($path);
    }
    elseif (!empty($input->string)) {
      $fingerprint = md5($input->string);
    }

    // Get filename.
    $output_basename = 'string';
    if (!empty($input->filename)) {
      $output_basename = basename($input->filename, '.css');
    }

    return "$fingerprint-$output_basename.css";
  }

  /**
   * Overrides CssCrush\IO::getCacheData().
   */
  public function getCacheData() {

    $data = cache_get('csscrush_fileinfo');
    return $data ? $data->data : array();
  }

  /**
   * Overrides CssCrush\IO::saveCacheData().
   */
  public function saveCacheData() {

    cache_set('csscrush_fileinfo', $this->process->cacheData, 'cache', CACHE_PERMANENT);
  }
}
