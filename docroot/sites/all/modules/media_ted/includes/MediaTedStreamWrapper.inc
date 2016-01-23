<?php

/**
 *  @file
 *  Create a Ted Stream Wrapper class for the Media/Resource module.
 */

/**
 *  Create an instance like this:
 *  $ted = new ResourceTedStreamWrapper('ted://?v=[video-code]');
 */
class MediaTedStreamWrapper extends MediaReadOnlyStreamWrapper {
  protected $base_url = 'http://ted.com/talks/view/id/';
  protected $embed_base = 'http://embed.ted.com/';

  function getTarget($f) {
    return FALSE;
  }

  static function getMimeType($uri, $mapping = NULL) {
    return 'video/ted';
  }

  function getOriginalThumbnailPath() {
    $parts = $this->get_parameters();

    $id = check_plain($parts['v']);

    if (is_numeric($id)) {
      return getOriginalEmbedsThumbnail($id);
    }

    $response = drupal_http_request($this->embed_base . $id);
    if ($response->code == 200) {
      // try to get thumbnail from actual iframe contents
      $pattern = '@(http[s]?://images\.ted\.com/[^\'"]*\.(gif|png|jpe?g))@i';
      if (preg_match($pattern, $response->data, $matches)) {
        return $matches[1];
      }
    }
  }

  function getLocalThumbnailPath() {
    $parts = $this->get_parameters();
    $local_path = 'public://media-ted/' . check_plain($parts['v']) . '.jpg';
    if (!file_exists($local_path)) {
      $dirname = drupal_dirname($local_path);
      file_prepare_directory($dirname, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
      @copy($this->getOriginalThumbnailPath(), $local_path);
    }
    return $local_path;
  }

  function getOriginalEmbedsThumbnail($id) {
    $url = 'http://www.ted.com/talks/embed/id/'. check_plain($id);
    $response = drupal_http_request($url);
    preg_match('@su=(http[s]?://.*\.(gif|png|jpg))@i', $response->data, $matches);
    return $matches[1];
  }
}
