<?php

/**
 * @file
 * Extends the MediaReadOnlyStreamWrapper class to handle Instagram objects.
 */

/**
 * Instagram stream wrapper.
 *
 * Create an instance like this:
 * $instagram = new MediaInstagramStreamWrapper('instagram://p/[code]');.
 */
class MediaInstagramStreamWrapper extends MediaReadOnlyStreamWrapper {
  protected $base_url = 'https://instagram.com/p';

  /**
   * Override MediaReadOnlyStreamWrapper::getMimeType().
   */
  public static function getMimeType($uri, $mapping = NULL) {
    return 'text/instagram';
  }

  /**
   * Override MediaReadOnlyStreamWrapper::interpolateUrl().
   */
  public function interpolateUrl() {
    if ($params = $this->get_parameters()) {
      return $this->base_url . '/' . $params['p'];
    }
    return FALSE;
  }
}
