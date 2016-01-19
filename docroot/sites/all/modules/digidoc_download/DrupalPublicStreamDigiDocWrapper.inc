<?php

/**
 * @file
 * Provides a redirect for ddoc files in the public stream.
 */

/**
 * Extends DrupalPublicStreamWrapper to redirect DigiDoc file downloads.
 */
class DrupalPublicStreamDigiDocWrapper extends DrupalPublicStreamWrapper {
  /**
   * Overrides getExternalUrl().
   */
  function getExternalUrl() {
    $path = str_replace('\\', '/', $this->getTarget());

    if($this->getMimeType($this->getUri()) == 'application/ddoc' || digidoc_download_is_ddoc($path)) {
      return url('digidoc/ddoc/public/' . $path, array('absolute' => TRUE));
    }

    return $GLOBALS['base_url'] . '/' . self::getDirectoryPath() . '/' . drupal_encode_path($path);
  }
}
