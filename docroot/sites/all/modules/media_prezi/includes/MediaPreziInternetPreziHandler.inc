<?php

/**
 * @file
 * Extends the MediaInternetBaseHandler class to handle Prezi presentations.
 */

/**
 * Implementation of MediaInternetBaseHandler.
 *
 * @see hook_media_internet_providers().
 */
class MediaPreziInternetPreziHandler extends MediaInternetBaseHandler {

  /**
   * Parse $embed_code to get uri.
   *
   * @param string $embed_code
   *   The emded code.
   *
   * @return string
   *   The normalized URI.
   */
  public function parse($embed_code) {
    $patterns = array(
      '@prezi\.com/embed/([a-zA-Z0-9-_]+)@i',
      '@prezi\.com/([a-zA-Z0-9-_]+)@i',
    );

    foreach ($patterns as $pattern) {
      preg_match($pattern, $embed_code, $matches);
      // @TODO: Parse is called often. Refactor so that valid ID is checked
      // when a presentation is added, not every time the embedCode is parsed.
      if (isset($matches[1]) && self::validId($matches[1])) {
        return file_stream_wrapper_uri_normalize('prezi://p/' . $matches[1]);
      }
    }
  }

  /**
   * Determines if this handler should claim the item.
   *
   * @param string $embed_code
   *   A string of user-submitted embed code.
   *
   * @return bool
   *   Pass TRUE to claim the item.
   */
  public function claim($embed_code) {
    if ($this->parse($embed_code)) {
      return TRUE;
    }
  }

  /**
   * Get prezi file.
   *
   * @return mixed
   *   A file.
   */
  public function getFileObject() {
    $uri = $this->parse($this->embedCode);
    $external_url = file_create_url($uri);
    $response = self::getPreziData($external_url);
    $file = file_uri_to_object($uri, TRUE);
    if (isset($response->redirect_url)) {
      preg_match('#([^/]+)\/$#', $response->redirect_url, $matches);

      // Try to default the file name to the presentation's title.
      if (isset($matches[1])) {
        $file->filename = str_replace('-', ' ', $matches[1]);
      }
    }
    return $file;
  }

  /**
   * Check if a Prezi presentation ID is valid.
   *
   * @return bool
   *   TRUE if the presentation ID is valid, or throws a
   *   MediaInternetValidationException otherwise.
   */
  static public function validId($id) {
    $uri = file_stream_wrapper_uri_normalize('prezi://p/' . check_plain($id));
      print_r($uri);
    $external_url = file_create_url($uri);
      print_r(' --> '.$external_url);
    $response = self::getPreziData($external_url);

    if ($response->code == 401) {
      throw new MediaInternetValidationException('Embedding has been disabled for this Prezi presentation.');
    }
    elseif ($response->code != 200) {
      throw new MediaInternetValidationException('The Prezi presentation ID is invalid or the presentation was deleted.');
    }
    return TRUE;
  }

  /**
   * Get presentation from prezi.
   *
   * @param string $url
   *   Url of prezi presentation.
   *
   * @return mixed
   *   Get presentation file.
   */
  static public function getPreziData($url) {
    static $_prezi_data = array();

    if (!isset($_prezi_data[$url])) {
      $_prezi_data[$url] = drupal_http_request($url);
    }
    return $_prezi_data[$url];
  }
}
