<?php

/**
 * @file media_8tracks/includes/MediaInterenet8TracksHandler.inc
 *
 * Contains MediaInternet8TracksHandler.
 */

/**
 * Implementation of MediaInternetBaseHandler.
 *
 * @see hook_media_internet_providers().
 */
class MediaInternet8TracksHandler extends MediaInternetBaseHandler {
  /**
   * The 8tracks id for this audio file.
   */
  private $id;

  /**
   * Check if a 8Tracks audio id is valid.
   *
   * Check against the oembed stream instead of the gdata api site to
   * avoid "yt:quota too_many_recent_calls" errors.
   *
   * @return
   *   Boolean.
   */
  static public function validId($id) {
    $xml = media_8tracks_get_info($id);
    if (isset($xml['mix']) && isset($xml['mix']['id'])) {
      return $xml['mix']['id'];
    }
    throw new MediaInternetValidationException("The 8Tracks audio ID is invalid or the audio file was deleted.");
  }

  public function parse($embedCode) {
    $patterns = array(
      '@8tracks\.com/([^"\& ]+)@i',
    );
    foreach ($patterns as $pattern) {
      preg_match($pattern, $embedCode, $matches);
      // @TODO: Parse is called often. Refactor so that valid ID is checked
      // when an audio file is added, but not every time the embedCode is
      // parsed.
      if (isset($matches[1]) && ($id = self::validId($matches[1]))) {
        return file_stream_wrapper_uri_normalize('8tracks://id/' . $id);
      }
    }
  }

  public function claim($embedCode) {
    if ($this->parse($embedCode)) {
      return TRUE;
    }
  }

  public function getFileObject() {
    $uri = $this->parse($this->embedCode);
    $file = file_uri_to_object($uri, TRUE);

    preg_match('@/id/(.+)@i', $uri, $matches);
    if (empty($file->fid) && isset($matches[1]) && $info = $this->getInfo($matches[1])) {
      $file->filename = truncate_utf8($info['mix']['name'], 255);
    }

    return $file;
  }

  /**
   * Returns information about the media.
   *
   * @return
   *   If information is available, a keyed array containing the information.
   *   Otherwise, FALSE.
   */
  public function getInfo($id) {
    return media_8tracks_get_info($id);
  }
}
