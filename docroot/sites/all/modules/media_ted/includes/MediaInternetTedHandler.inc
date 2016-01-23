<?php

/**
 * Implementation of MediaInternetBaseHandler.
 *
 * @see hook_media_internet_providers().
 */
class MediaInternetTedHandler extends MediaInternetBaseHandler {
  public function parse($embedCode) {
    if (preg_match('@\[ted id=([0-9]+).*\]@i', $embedCode, $matches) && isset($matches[1])) {
      $id = $matches[1];
      $lang = NULL;

      if (preg_match('@lang=([a-z]{2}).*@i', $embedCode, $matches)) {
        $lang = $matches[1];
      }

      $embedCode = $this->getEmbedFromId($id, $lang);
    }
    $patterns = array(
      '@ted\.com\/(talks\/[^\'"]*)@i',
    );

    foreach ($patterns as $pattern) {
      if (preg_match($pattern, $embedCode, $matches) && isset($matches[1])) {
        return file_stream_wrapper_uri_normalize('ted://v/' . urlencode($matches[1]));
      }
    }
  }

  public function getEmbedFromId($id, $language = NULL) {
    $url = 'http://www.ted.com/talks/embed/id/'. $id;
    if ($language) {
      $url .= '/lang/'. check_plain($language);
    }

    $response = drupal_http_request($url);
    if ($response->code != 200) {
      throw new MediaInternetValidationException(t('The TED video ID is invalid or the video was deleted.'));
    }
    return $response->data;
  }

  public function claim($embedCode) {
    if ($this->parse($embedCode)) {
      return TRUE;
    }
  }

  public function save() {
    $file = $this->getFileObject();
    // If a user enters a duplicate Ted URL, the object will be saved again.
    // Set the timestamp to the current time, so that the media item shows up
    // at the top of the media library, where they would expect to see it.
    $file->timestamp = REQUEST_TIME;
    file_save($file);
    return $file;
  }

  public function getFileObject() {
    $uri = $this->parse($this->embedCode);
    return file_uri_to_object($uri, TRUE);
  }

  /**
   * Returns information about the media. See http://video.search.yahoo.com/mrss.
   *
   * @return
   *   If ATOM+MRSS information is available, a SimpleXML element containing
   *   ATOM and MRSS elements, as per those respective specifications.
   *
   * @todo Would be better for the return value to be an array rather than a
   *   SimpleXML element, but media_retrieve_xml() needs to be upgraded to
   *   handle namespaces first.
   */
  public function getMRSS() {
    return FALSE;
  }

  /**
   * Returns information about the media. See http://www.oembed.com/.
   *
   * @return
   *   If oEmbed information is available, an array containing 'title', 'type',
   *   'url', and other information as specified by the oEmbed standard.
   *   Otherwise, NULL.
   */
  public function getOEmbed() {
    return FALSE;
  }
}
