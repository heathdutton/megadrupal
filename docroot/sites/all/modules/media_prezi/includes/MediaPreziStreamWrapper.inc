<?php

/**
 * @file
 * Extends the MediaReadOnlyStreamWrapper class to handle Prezi presentations.
 */

/**
 * Create an instance.
 *
 * Ex: $prezi = new MediaPreziStreamWrapper('prezi://[presentation-code]');.
 */
class MediaPreziStreamWrapper extends MediaReadOnlyStreamWrapper {
  protected $baseUrl = 'https://prezi.com/embed';

  /**
   * Modify the url with parameters.
   *
   * @return string
   *   Url with parameters.
   */
  public function interpolateUrl() {
    if ($parameters = $this->get_parameters()) {
      return $this->baseUrl . '/' . $parameters['p'];
    }
  }

  /**
   * Get mime type.
   *
   * @param string $uri
   *   Uri to the file.
   * @param mixed $mapping
   *   An optional map of extensions to their mimetypes.
   *
   * @return string
   *   Get the mime type for prezi presentation.
   */
  public static function getMimeType($uri, $mapping = NULL) {
    return 'presentation/prezi';
  }

  /**
   * Get thumbnail path.
   *
   * @return string
   *   Path.
   */
  public function getOriginalThumbnailPath() {
    return drupal_get_path('module', 'media_prezi') . '/images/preso.png';
  }

  /**
   * Get path to thumbnails.
   *
   * @return string
   *   Path.
   */
  public function getLocalThumbnailPath() {
    $parts = $this->get_parameters();
    // There's no need to hide thumbnails, always use the public system rather.
    // than file_default_scheme().
    $local_path = 'public://media-prezi/' . check_plain($parts['p']) . '.jpg';

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
