<?php


/**
 * @file
 * Contains MediaInternet23VideoHandler.
 */

/**
 * Implementation of MediaInternetBaseHandler.
 *
 * @see hook_media_internet_providers().
 */
class MediaInternet23VideoHandler extends MediaInternetBaseHandler {

  // Used as a static cache to improve the performance.
  protected static $processedEmbedCodes = array();

  /**
   * Check if a 23Video video id is valid.
   */
  public function parse($embed_code) {
    if (isset(self::$processedEmbedCodes[$embed_code], self::$processedEmbedCodes[$embed_code][__METHOD__])) {
      return self::$processedEmbedCodes[$embed_code][__METHOD__];
    }

    $embed_code = rawurldecode($embed_code);
    $patterns = array(
      '@src="(([^"]+23video\.com)([^"]+))@i',
    );
    foreach ($patterns as $pattern) {
      if (preg_match($pattern, $embed_code, $matches)) {
        $url_parts = parse_url($matches[1]);
        if (isset($url_parts['query'])) {
          parse_str($url_parts['query'], $query);
          if (isset($query['photo_id'])) {
            $embed_code = $matches[2] . '/video/' . $query['photo_id'];
            $this->embedCode = $embed_code;
          }
        }
      }

    }

    $video_info = $this->getOEmbed();
    if ($video_info && isset($video_info['photo_id'], $video_info['provider_url']) && $video_info['type'] == 'video' && !empty($video_info['photo_id'])) {
      self::$processedEmbedCodes[$embed_code][__METHOD__] = file_stream_wrapper_uri_normalize('23video://v/' . $video_info['photo_id'] . '/s/' . parse_url($video_info['provider_url'], PHP_URL_HOST));
      return self::$processedEmbedCodes[$embed_code][__METHOD__];
    }
    self::$processedEmbedCodes[$embed_code][__METHOD__] = FALSE;
    return self::$processedEmbedCodes[$embed_code][__METHOD__];
  }

  /**
   * Determines if this handler should claim the item.
   * @see MediaInternetBaseHandler::claim()
   */
  public function claim($embed_code) {
    if ($this->parse($embed_code)) {
      return TRUE;
    }
  }

  /**
   * Returns a file object which can be used for validation.
   * @see MediaInternetBaseHandler::getFileObject()
   */
  public function getFileObject() {
    $uri = $this->parse($this->embedCode);
    $file = file_uri_to_object($uri, TRUE);

    if (empty($file->fid) && $info = $this->getOEmbed()) {
      $file->filename = truncate_utf8($info['title'], 255);
    }

    return $file;
  }

  /**
   * Returns information about the media. See http://www.oembed.com/.
   */
  public function getOEmbed($params = array()) {
    if (isset(self::$processedEmbedCodes[$this->embedCode], self::$processedEmbedCodes[$this->embedCode][__METHOD__])) {
      return self::$processedEmbedCodes[$this->embedCode][__METHOD__];
    }
    $info = NULL;
    $url_parts = parse_url($this->embedCode);
    if (isset($url_parts['host'])) {
      $params += array('url' => $this->embedCode, 'format' => 'json');
      $oembed_url = url('http://' . $url_parts['host'] . '/oembed', array('query' => $params));
      $response = drupal_http_request($oembed_url);
      if (!isset($response->error)) {
        $info = drupal_json_decode($response->data);
      }
    }

    self::$processedEmbedCodes[$this->embedCode][__METHOD__] = $info;
    return $info;
  }

}
