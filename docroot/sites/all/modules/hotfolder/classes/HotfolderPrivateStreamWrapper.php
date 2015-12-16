<?php
/**
 * @file - A class to implement the Hotfolders specific streams.
 */

class HotfolderPrivateStreamWrapper extends DrupalPrivateStreamWrapper {
  /**
   * Implements abstract public function getDirectoryPath()
   */
  public function getDirectoryPath() {
    $uri = $this->getUri();
    // We only know exactly which one to go with if we have a uri.
    if (!empty($uri)) {
      $stream = file_uri_scheme($this->uri) . '://';
      $hotfolder_streams = _hotfolder_get_schemas();
      if (isset($hotfolder_streams[$stream]) && !empty($hotfolder_streams[$stream])) {
        return _hotfolder_realpath($hotfolder_streams[$stream]);
      }
    }
    // Fallback to private.
    return variable_get('file_private_path', '');
  }

  /**
   * Returns the canonical absolute path of the URI, if possible.
   */
  public function getLocalPath($uri = NULL) {
    if (!isset($uri)) {
      $uri = $this->uri;
    }
    // PHP realpath would normally be used here, but we have to
    // account for UNC paths in this situation.
    $path = $this->getDirectoryPath() . DIRECTORY_SEPARATOR . $this->getTarget($uri);
    // Check the directory first.
    $directory = realpath($this->getDirectoryPath());
    if (!$directory) {
      // Check for parent UNC directory.
      // This function returns FALSE for files inaccessible in safe mode.
      $dir_path = $this->getDirectoryPath();
      // Translate to the local directory separator.
      $dir_path = _hotfolder_realpath($dir_path);
      if (file_exists($dir_path)) {
        $directory = $dir_path;
      }
    }
    // PHP realpath ONLY works in the context of having a
    // local file - it doesn't work with remote systems like UNC paths.
    // At this point either this is a remote sys OR the file isn't there.
    $realpath = realpath($path);
    // But we don't want to go on blind faith that this path actually exists.
    if (!$realpath) {
      // By this point in the call, all schemes ARE registered with PHP.
      // So fopen and other functions work as expected with UNC paths.
      // This function returns FALSE for files inaccessible in safe mode.
      // Translate to the local directory separator.
      $path = _hotfolder_realpath($path);
      if (file_exists($path)) {
        $realpath = $path;
      }
      else {
        // This file does not yet exist.
        $realpath = $directory . DIRECTORY_SEPARATOR . _hotfolder_realpath($this->getTarget($uri));
      }
    }
    // Must be a file and on the correct stream path.
    if (!$realpath || !$directory || strpos($realpath, $directory) !== 0) {
      return FALSE;
    }
    return _hotfolder_realpath($realpath);
  }
}
