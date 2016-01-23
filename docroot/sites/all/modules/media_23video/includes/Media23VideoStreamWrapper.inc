<?php

/**
 * @file
 * Create a 23video Stream Wrapper class for the Media/Resource module.
 */


/**
 *  Create an instance
 */
class Media23VideoStreamWrapper extends MediaReadOnlyStreamWrapper {

  /**
   * Handles parameters on the URL string.
   * @see MediaReadOnlyStreamWrapper::interpolateUrl()
   */
  public function interpolateUrl() {
    if ($parameters = $this->get_parameters()) {
      return "http://{$parameters['s']}/v.ihtml?source=share&photo_id={$parameters['v']}&autoPlay=0";
    }
  }

  /**
   * Returns the MIME type of the resource.
   * @see MediaReadOnlyStreamWrapper::getMimeType()
   */
  public static function getMimeType($uri, $mapping = NULL) {
    return 'video/23video';
  }

  /**
   * Returns original thumbnail.
   */
  public function getOriginalThumbnailPath() {
    if ($parameters = $this->get_parameters()) {
      $video_handler = new MediaInternet23VideoHandler("http://{$parameters['s']}/video/{$parameters['v']}");
      $info = $video_handler->getOEmbed();
      if (empty($info)) {
        return;
      }

      return $info['thumbnail_url'];
    }
  }

  /**
   * Returns local thumbnail.
   */
  public function getLocalThumbnailPath() {
    $parts = $this->get_parameters();
    // There's no need to hide thumbnails, always use the public system rather
    // than file_default_scheme().
    $local_path = 'public://media-23video/' . check_plain($parts['v']) . '.jpg';
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
