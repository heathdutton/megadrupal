<?php

/**
 * @file media_issuu/includes/MediaInternetIssuuHandler.inc
 * media_internet handler for Issuu.com.
 */

/**
 * Implementation of MediaInternetBaseHandler.
 *
 * @see hook_media_internet_providers()
 */
class MediaInternetIssuuHandler extends MediaInternetBaseHandler {

  /**
   * Check if a Issuu.com id is valid.
   * @todo implement function.
   *
   * @return
   *   Boolean.
   */
  static public function validURL($url) {
    $response = drupal_http_request($url, array('method' => 'HEAD'));
    if ($response->code == 404) {
      throw new MediaInternetValidationException("Issuu cannot be found. ");
    }
    return TRUE;
  }

  /**
   * Parse URL from Issuu.
   *
   * Issuu has following URL format:
   * http://issuu.com/<user>/docs/<doctitle>
   */
  public function parse($embedCode) {
    // Attempt to match on a URL.
    $url_patterns = array(
      '@issuu\.com/(.+)/docs/([^?]+)@i',
      '@static\.issuu\.com/webembed/viewers/style1/v1/IssuuViewer\.swf\?(?=.*?username\=([^\&]*))(?=.*?docName\=([^\&]*)).*@i',
    );
    foreach ($url_patterns as $pattern) {
      preg_match($pattern, $embedCode, $matches);
      // @TODO: Parse is called often. Refactor so that valid ID is checked
      // when a video is added, but not every time the embedCode is parsed.
      if (isset($matches[0]) && isset($matches[1]) && isset($matches[2]) && self::validURL($embedCode)) {
        return file_stream_wrapper_uri_normalize('issuu://' . $matches[1] . '/docs/' . $matches[2]);
      }
    }

    // Attempt to match using an embed code.
    $embed_patterns = array(
      '@data-configid="(\d+)/(\d+)"@i',
    );
    foreach ($embed_patterns as $pattern) {
      preg_match($pattern, $embedCode, $matches);
      // @TODO: Parse is called often. Refactor so that valid ID is checked
      // when a video is added, but not every time the embedCode is parsed.
      if (isset($matches[0]) && isset($matches[1]) && isset($matches[2]) && self::validURL($embedCode)) {
        // If we find a match based on the embed numeric IDs, we need to fetch
        // more info from the API.
        $embed_info_url = 'http://e.issuu.com/embed/' . $matches[1] . '/' . $matches[2] . '.json';
        $embed_info_response = drupal_http_request($embed_info_url);

        // If we can a valid response, generate a stream URI.
        if ($embed_info_response->code == 200) {
          $embed_info = json_decode($embed_info_response->data);

          // We require the Document user (du) and the Document Name (dn).
          if (isset($embed_info->du) && isset($embed_info->dn)) {
            return file_stream_wrapper_uri_normalize('issuu://' . $embed_info->du . '/docs/' . $embed_info->dn);
          }
        }
      }
    }
  }

  /**
   * Get file object.
   */
  public function getFileObject() {
    $uri = $this->parse($this->embedCode);
    $file = file_uri_to_object($uri, TRUE);
    // Try to default the file name to the issuu's title.
    $response = drupal_http_request($this->embedCode);
    if (!isset($response->error)) {
      // get <meta property="og:title" content="Dubbelpunt oktober2013 FC de leidingsploeg"> form $response->data
      $doc = new DOMDocument();
      libxml_use_internal_errors(true);
      $doc->loadHTML($response->data);
      libxml_clear_errors();
      foreach ($doc->getElementsByTagName('meta') as $meta) {
        if ($meta->getAttribute('property') == 'og:title') {
          $title = $meta->getAttribute('content');
          $file->filename = truncate_utf8($title, 255);
        }
      }
    }
    return $file;
  }

  /**
   * Claim parsed embedCode.
   */
  public function claim($embed_code) {
    if ($this->parse($embed_code)) {
      return TRUE;
    }
  }

  /**
   * Save file.
   */
  public function save() {
    $file = $this->getFileObject();
    file_save($file);
    return $file;
  }

}
