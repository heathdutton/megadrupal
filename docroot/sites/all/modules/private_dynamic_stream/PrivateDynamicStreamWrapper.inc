<?php

/**
 * Private dynamic (private_dynamic://) stream wrapper class.
 *
 * Provides support for storing privately accessible dyncamic files with the
 * Drupal file interface.
 *
 * Extends DrupalLocalStreamWrapper.
 */
class PrivateDynamicStreamWrapper extends DrupalLocalStreamWrapper {
  /**
   * Implements abstract public function getDirectoryPath()
   */
  public function getDirectoryPath() {
    return variable_get('private_dynamic_stream_path', '');
  }

  /**
   * Overrides getExternalUrl().
   *
   * Return the HTML URI of a private dynamic file.
   */
  function getExternalUrl() {
    $path = str_replace('\\', '/', $this->getTarget());
    return url('system/dynamics/' . $path, array('absolute' => TRUE));
  }
}
