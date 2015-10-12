<?php

/**
 * @file
 * Extends the MediaInternetBaseHandler class to handle Solidtango videos.
 */

/**
 * Implementation of MediaInternetBaseHandler.
 *
 * @see hook_media_internet_providers().
 */
class MediaInternetSolidtangoHandler extends MediaInternetBaseHandler {
  /**
   * Parse the embed url.
   *
   * @return string
   *   Complete solidtango url with host, type and slug.
   */
  public function parse($embed_code) {
    // http://example.solidtango.com/video/x1y2z3a4
    $patterns = array(
      '@([a-z0-9\-]+\.solidtango\.com)/(video)/(.+)@i',
      '@([a-z0-9\-]+\.solidtango\.com)/(live)/(.+)@i',
    );

    foreach ($patterns as $pattern) {
      preg_match($pattern, $embed_code, $matches);
      // @TODO: Parse is called often. Refactor so that valid ID is checked
      // when a video is added, but not every time the embed_code is parsed.
      if (isset($matches[1]) && isset($matches[2]) && isset($matches[3]) && self::validId($matches[1], $matches[2], isset($matches[3]))) {
        return file_stream_wrapper_uri_normalize('solidtango://host/' . $matches[1] . '/type/' . $matches[2] . '/slug/' . $matches[3]);
      }
    }
  }

  /**
   * Override inherited claim.
   */
  public function claim($embed_code) {
    if ($this->parse($embed_code)) {
      return TRUE;
    }
  }

  /**
   * Override inherited getFileObject.
   */
  public function getFileObject() {
    $uri = $this->parse($this->embedCode);
    $file = file_uri_to_object($uri, TRUE);

    // Try to default the file name to the video's title.
    if (empty($file->fid) && $info = $this->getOEmbed()) {
      $file->filename = truncate_utf8($info['title'], 255);
    }

    return $file;
  }

  /**
   * Returns information about the media.
   *
   * See http://www.oembed.com.
   *
   * @return array|null
   *   If oEmbed information is available, an array containing 'title', 'type',
   *   'url', and other information as specified by the oEmbed standard.
   *   Otherwise, NULL.
   */
  public function getOEmbed() {
    $uri = $this->parse($this->embedCode);
    $external_url = file_create_url($uri);
    $oembed_url = url('http://solidtango.com/api/v1/oembed_video.json', array('query' => array('url' => $external_url)));
    $response = drupal_http_request($oembed_url);

    if (!isset($response->error)) {
      return drupal_json_decode($response->data);
    }
    else {
      throw new Exception("Error Processing Request. (Error: {$response->code}, {$response->error})");
      return;
    }
  }

  /**
   * Check if a Solidtango video ID is valid.
   *
   * @return bool
   *   TRUE if the video ID is valid, or throws a
   *   MediaInternetValidationException otherwise.
   */
  static public function validId($host, $type, $id) {
    $uri = file_stream_wrapper_uri_normalize('solidtango://host/' . check_plain($host) . '/type/' . check_plain($type) . '/slug/' . check_plain($id));
    $external_url = file_create_url($uri);
    $oembed_url = url('http://solidtango.com/api/v1/oembed_video.json', array('query' => array('url' => $external_url)));
    $response = drupal_http_request($oembed_url, array('method' => 'HEAD'));

    if ($response->code == 401) {
      throw new MediaInternetValidationException('Embedding has been disabled for this Solidtango video.');
    }
    elseif ($response->code != 200) {
      throw new MediaInternetValidationException('The Solidtango video ID is invalid or the video was deleted.');
    }

    return TRUE;
  }
}
