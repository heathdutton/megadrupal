<?php

/**
 *  @file media_8tracks/includes/Media8TracksStreamWrapper.inc
 *
 *  Create a 8Tracks Stream Wrapper class for the Media/Resource module.
 */

/**
 *  Create an instance like this:
 *  $8tracks = new Media8TracksStreamWrapper('8tracks://v/[audio-code]');
 */
class Media8TracksStreamWrapper extends MediaReadOnlyStreamWrapper {

  // Overrides $base_url defined in MediaReadOnlyStreamWrapper.
  protected $base_url = 'http://8tracks.com/';

  /**
   * Returns a url in the format "http://8tracks.com/mixes/1234".
   *
   * Overrides interpolateUrl() defined in MediaReadOnlyStreamWrapper.
   * This is an exact copy of the function in MediaReadOnlyStreamWrapper,
   * here in case that example is redefined or removed.
   */
  function interpolateUrl() {
    if ($parameters = $this->get_parameters()) {
      return $this->base_url . '/mixes/' . http_build_query($parameters);
    }
  }

  static function getMimeType($uri, $mapping = NULL) {
    return 'audio/8tracks';
  }

  function getTarget($f) {
    return FALSE;
  }

  function getOriginalThumbnailPath() {
    $parts = $this->get_parameters();
    $info = media_8tracks_get_info($parts['id']);
    if (isset($info['mix']) && isset($info['mix']['cover-urls']) && isset($info['mix']['cover-urls']['original'])) {
      return $info['mix']['cover-urls']['original'];
    }
  }

  function getLocalThumbnailPath() {
    $parts = $this->get_parameters();
    // There's no need to hide thumbnails, always use the public system rather
    // than file_default_scheme().
    $local_path = 'public://media-8tracks/' . check_plain($parts['id']) . '.jpg';
    if (!file_exists($local_path)) {
      $dirname = drupal_dirname($local_path);
      file_prepare_directory($dirname, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
      $response = drupal_http_request($this->getOriginalThumbnailPath());
      if (!isset($response->error)) {
        file_unmanaged_save_data($response->data, $local_path, TRUE);
      }
      else {
        @copy($this->getOriginalThumbnailPath(), $local_path);
      }
    }
    return $local_path;
  }
}
