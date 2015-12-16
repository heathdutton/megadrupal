<?php

/**
 * @file
 * Extends the MediaInternetBaseHandler class for Vine.
 */

/**
 * Implementation of MediaInternetBaseHandler.
 *
 * @see hook_media_internet_providers().
 */
class MediaInternetVineHandler extends MediaInternetBaseHandler {

  /**
   * Parse given Vine url.
   */
  public function parse($embedCode) {
    // E.g: https://vine.co/v/hrAOEOBw0F2
    $pattern = '%vine\.co\/v\/(.*)%i';
    preg_match($pattern, $embedCode, $matches);
    if (isset($matches[1]) && self::validId($matches[1])) {
      return file_stream_wrapper_uri_normalize('vine://v/' . $matches[1]);
    }
  }

  /**
   * Determines if this handler should claim the item.
   */
  public function claim($embedCode) {
    if ($this->parse($embedCode)) {
      return TRUE;
    }
  }

  /**
   * Get the file object.
   */
  public function getFileObject() {
    $uri = $this->parse($this->embedCode);
    $file = file_uri_to_object($uri, TRUE);
    // Try to default the file name to the video's title.
    if (empty($file->fid) && $info = $this->getOEmbed()) {
      // Remove non-alphanumeric characters (e.g: 😍)
      $vine_description = preg_replace('/[^a-z\d ]/i', '', check_plain($info['data']['records'][0]['description']));
      $file->filename = truncate_utf8($vine_description, 255);
    }

    return $file;
  }

  /**
   * Returns information about the media.
   *
   * See http://www.oembed.com.
   *
   * @return mixed
   *   If oEmbed information is available, an array containing 'title', 'type',
   *   'url', and other information as specified by the oEmbed standard.
   *   Otherwise, NULL.
   */
  public function getOEmbed() {
    $uri = $this->parse($this->embedCode);
    $video_id = arg(1, file_uri_target($uri));
    $oembed_url = url('https://api.vineapp.com/timelines/posts/s/' . $video_id);
    $response = drupal_http_request($oembed_url);

    if (!isset($response->error)) {
      return drupal_json_decode($response->data);
    }

    throw new Exception("Error Processing Request. (Error: {$response->code}, {$response->error})");
  }

  /**
   * Check if a Vine video ID is valid.
   *
   * @return bool
   *   TRUE if the video ID is valid, or throws a
   *   MediaInternetValidationException otherwise.
   */
  static public function validId($id) {
    $url = 'https://api.vineapp.com/timelines/posts/s/' . $id;
    $response = drupal_http_request($url, array('method' => 'HEAD'));

    if ($response->code == 401) {
      throw new MediaInternetValidationException('Embedding has been disabled for this Vine video.');
    }
    elseif ($response->code != 200) {
      throw new MediaInternetValidationException('The Vine video ID is invalid or the video was deleted.');
    }

    return TRUE;
  }

}
