<?php

/**
 * @file
 * Definition of MediaKalturaStreamWrapper.
 */

class MediaKalturaStreamWrapper extends MediaReadOnlyStreamWrapper {
  protected $base_url = 'http://www.kaltura.com/';

  function interpolateUrl() {
    return $this->base_url . $this->get_parameters();
  }

  function getTarget($f) {
    return FALSE;
  }

  function getExternalUrl() {
    // Return the direct image.
    $parts = $this->get_parameters();
    $url_pieces = explode('/', $parts);
    $partner_id = str_replace('_', '', $url_pieces[3]);
    return $this->base_url . 'p/' . $partner_id . '/thumbnail/entry_id/' . $url_pieces[7] . '/width/1000/height/1000';
  }

  function getOriginalThumbnailPath() {
    return $this->getExternalUrl();
  }

  function getLocalThumbnailPath() {
    $local_path = 'public://kaltura/' . $this->get_parameters() . '.jpg';
    if (!file_exists($local_path)) {
      $dirname = drupal_dirname($local_path);
      file_prepare_directory($dirname, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
      @copy($this->getOriginalThumbnailPath(), $local_path);
    }
    return $local_path;
  }

  static function getMimeType($uri, $mapping = NULL) {
    return 'video/kaltura';
  }

  function setUri($uri) {
    parent::setUri($uri);
  }

  protected function _parse_url($url) {
    $path = explode('://', $url);
    return $path[1];
  }
}
