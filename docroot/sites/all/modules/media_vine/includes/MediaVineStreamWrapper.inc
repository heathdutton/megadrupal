<?php

/**
 * @file
 * Extends the MediaReadOnlyStreamWrapper class for Vine.
 */

/**
 * Implementation of MediaReadOnlyStreamWrapper.
 *
 * Create an instance like this:
 * $vine = new MediaVineStreamWrapper('vine://v/[video-id]');
 */
class MediaVineStreamWrapper extends MediaReadOnlyStreamWrapper {

  protected $base_url = 'http://vine.com';

  /**
   * Get the Vine TimeType.
   */
  public static function getMimeType($uri, $mapping = NULL) {
    return 'video/vine';
  }

  /**
   * Handles parameters on the URL string.
   *
   * Rewrite the URL before is it passed back.
   */
  public function interpolateUrl() {
    if ($params = $this->get_parameters()) {
      return $this->base_url . '/' . $params['v'];
    }
  }

  /**
   * Get the original thumbnail path provided by Vine api.
   */
  public function getOriginalThumbnailPath() {
    $parts = $this->get_parameters();
    $oembed_url = url('https://api.vineapp.com/timelines/posts/s/' . $parts['v']);
    $response = drupal_http_request($oembed_url);

    if (!isset($response->error)) {
      $data = drupal_json_decode($response->data);
      return $data['data']['records'][0]['thumbnailUrl'];
    }
    throw new Exception("Error Processing Request. (Error: {$response->code}, {$response->error})");
  }

  /**
   * Get the local thumbnail.
   *
   * Attempt to get the local thumbnail, if it's not there, fetch it
   * from remote thumbnail path.
   */
  public function getLocalThumbnailPath() {
    $parts = $this->get_parameters();
    // There's no need to hide thumbnails, always use the public system rather
    // than file_default_scheme().
    $local_path = 'public://media-vine/' . check_plain($parts['v']) . '.jpg';

    if (!file_exists($local_path)) {
      $dirname = drupal_dirname($local_path);
      file_prepare_directory($dirname, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
      $response = drupal_http_request($this->getOriginalThumbnailPath());

      if (!isset($response->error)) {
        file_unmanaged_save_data($response->data, $local_path, TRUE);
      }
      else {
        copy($this->getOriginalThumbnailPath(), $local_path);
      }
    }

    return $local_path;
  }

}
