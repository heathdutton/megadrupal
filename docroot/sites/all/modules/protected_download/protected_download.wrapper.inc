<?php

/**
 * @file
 * Contains ProtectedDownloadStreamWrapper.
 */

/**
 * A stream wrapper leveraging the protected download module.
 */
class ProtectedDownloadStreamWrapper extends DrupalLocalStreamWrapper {

  /**
   * Implements abstract public function getDirectoryPath().
   */
  public function getDirectoryPath() {
    return variable_get('protected_download_file_path_protected', '');
  }

  /**
   * Overrides getExternalUrl().
   *
   * Return the HTML URI of a protected file.
   */
  public function getExternalUrl() {
    return protected_download_create_url($this->uri);
  }

}
