<?php

/**
 * @file
 * Extends the MediaInternetBaseHandler class to handle SoundCloud audio.
 */

/**
 * Implementation of MediaInternetBaseHandler.
 *
 * @see hook_media_internet_providers().
 */
class MediaInternetSoundCloudHandler extends MediaInternetBaseHandler {
  public function parse($embedCode) {
    $patterns = array(
      '@soundcloud\.com/([0-9A-Za-z\@\&\$_-]+)/([0-9A-Za-z\@\&\$_-]+)/([0-9A-Za-z\@\&\$_-]+)@i',
      '@soundcloud\.com/([0-9A-Za-z\@\&\$_-]+)/([0-9A-Za-z\@\&\$_-]+)@i',
      '@soundcloud\.com/([0-9A-Za-z\@\&\$_-]+)@i',
    );

    foreach ($patterns as $pattern) {
      $uri = FALSE;

      // @TODO: Parse is called often. Refactor so that valid ID is checked
      // when a song is added, but not every time the embedCode is parsed.
      preg_match($pattern, $embedCode, $matches);

      if (isset($matches[1]) && !isset($matches[2])) {
        $uri = 'soundcloud://u/' . $matches[1];
      }

      if (isset($matches[2]) && !isset($matches[3])) {
        if ($matches[1] == 'groups') {
          $uri = 'soundcloud://g/' . $matches[2];
        }
        else{
          $uri = 'soundcloud://u/' . $matches[1] . '/a/' . $matches[2];
        }
      }

      if (isset($matches[3])) {
        if ($matches[2] == 'sets') {
          $uri = 'soundcloud://u/' . $matches[1] . '/s/' . $matches[3];
        }
      }

      if ($uri && self::validId($uri)) {
        return file_stream_wrapper_uri_normalize($uri);
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
   * @return
   *   If oEmbed information is available, an array containing 'title', 'type',
   *   'url', and other information as specified by the oEmbed standard.
   *   Otherwise, NULL.
   */
  public function getOEmbed() {
    $uri = $this->parse($this->embedCode);
    $external_url = file_create_url($uri);
    $oembed_url = url('http://soundcloud.com/oembed', array('query' => array('url' => $external_url, 'format' => 'json')));
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
   * Check if a SoundCloud audio ID is valid.
   *
   * @return boolean
   *   TRUE if the audio ID is valid, or throws a
   *   MediaInternetValidationException otherwise.
   */
  static public function validId($uri) {
    $uri = file_stream_wrapper_uri_normalize($uri);
    $external_url = file_create_url($uri);
    $oembed_url = url('http://soundcloud.com/oembed', array('query' => array('url' => $external_url, 'format' => 'json')));
    $response = drupal_http_request($oembed_url, array('method' => 'HEAD'));

    if ($response->code == 401) {
      throw new MediaInternetValidationException('Embedding has been disabled for this SoundCloud audio.');
    }
    elseif ($response->code != 200) {
      throw new MediaInternetValidationException('The SoundCloud audio ID is invalid or the audio is either private or has been deleted.');
    }

    return TRUE;
  }
}
