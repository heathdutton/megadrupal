<?php

/**
 * @file
 * Provides a redirect for ddoc files in the private stream.
 */

/**
 * Extends DrupalPrivateStreamWrapper to redirect DigiDoc file downloads.
 */
class DrupalPrivateStreamDigiDocWrapper extends DrupalPrivateStreamWrapper {
  /**
   * Overrides getExternalUrl().
   */
  function getExternalUrl() {
    $path = str_replace('\\', '/', $this->getTarget());
    if($this->getMimeType($this->getUri()) == 'application/ddoc' || digidoc_download_is_ddoc($path)) {
      return url('digidoc/ddoc/private/' . $path, array('absolute' => TRUE));
    }

    return url('system/files/' . $path, array('absolute' => TRUE));
  }
}
