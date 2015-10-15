<?php

/**
 * @file
 * Extends the MediaInternetBaseHandler class to handle Instagram objects.
 */

/**
 * Implementation of MediaInternetBaseHandler.
 */
class MediaInternetInstagramHandler extends MediaInternetBaseHandler {
  private $instagramUrl = 'https://instagram.com/p';
  private $oembedUrl = 'http://api.instagram.com/publicapi/oembed/';
  private $uri = FALSE;

  /**
   * Override MediaInternetBaseHandler::__construct().
   */
  public function __construct($url) {
    parent::__construct($url);

    $patterns = array(
      '@instagram\.com/p/([^"\&\? ]+)@i',
      '@instagr\.am/p/([^"\&\? ]+)@i',
    );

    foreach ($patterns as $pattern) {
      preg_match($pattern, $url, $matches);
      if (isset($matches[1])) {
        $short_code = rtrim($matches[1], '/');
        if ($this->isValidShortCode($short_code)) {
          $this->uri = file_stream_wrapper_uri_normalize('instagram://p/' . $short_code);
          break;
        }
      }
    }
  }

  /**
   * Override MediaInternetBaseHandler::claim().
   */
  public function claim($url) {
    if ($this->uri) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Override MediaInternetBaseHandler::getFileObject().
   */
  public function getFileObject() {
    $file = file_uri_to_object($this->uri, TRUE);

    // Try to default the file name instagram object title.
    if (empty($file->fid) && $data = $this->getOEmbed()) {
      $file->filename = truncate_utf8($data['title'], 255);
    }

    return $file;
  }

  /**
   * Return an URI by making it syntactically correct.
   *
   * @return string
   *   The normalized URI or FALSE.
   */
  public function parse() {
    return $this->uri;
  }

  /**
   * Returns information about the media.
   *
   * @param array $query
   *   Query params for request. You can see supported params here:
   *   https://instagram.com/developer/embedding/#oembed.
   *
   * @return array
   *   If oEmbed information is available, an array containing 'title', 'type',
   *   'url', and other information as specified by the oEmbed standard.
   *   Otherwise, NULL.
   *
   * @throws Exception
   */
  public function getOEmbed($query = array()) {
    if ($this->uri) {
      $query['url'] = file_create_url($this->uri);
      $url = url($this->oembedUrl, array('query' => $query));
      $response = drupal_http_request($url);
      if (!isset($response->error)) {
        return drupal_json_decode($response->data);
      }
    }
    return FALSE;
  }

  /**
   * Check if a Instagram url is valid.
   *
   * @param string $short_code
   *   The sort code of the Instagram object.
   *
   * @return bool
   *   TRUE if the url is valid, FALSE otherwise.
   */
  private function isValidShortCode($short_code) {
    $query = array('url' => $this->instagramUrl . '/' . $short_code);
    $url = url($this->oembedUrl, array('query' => $query));
    $response = drupal_http_request($url);

    if ($response->code == 401) {
      watchdog('php', 'Instagram object has been disabled.', array(), WATCHDOG_ERROR);
      return FALSE;
    }
    if ($response->code != 200) {
      watchdog('php', 'The path is invalid or the instagram object was deleted.', array(), WATCHDOG_ERROR);
      return FALSE;
    }

    return TRUE;
  }
}
