<?php

/**
 * @file
 * Extends the MediaInternetBaseHandler class to handle ShareStream videos.
 */

/**
 * Implementation of MediaInternetBaseHandler.
 *
 * @see hook_media_internet_providers().
 */
class MediaInternetShareStreamHandler extends MediaInternetBaseHandler {
  public function parse($embedCode) {
    $server = variable_get('media_sharestream_server');

    $server = str_replace('.', '\.', $server);

    // https://example.org/ssdcms/i.do?u=*
    // https://example.org/ssdcms/ip.do?u=8ea38c53cfa5439
    // https://example.org/ssdcms/ipublic.do?u=8ea38c53cfa5439
    $patterns = array(
      '@' . $server . '/ssdcms/i\.do\?u=([^"\&\? ]+)@i',
      '@' . $server . '/ssdcms/ip\.do\?u=([^"\&\? ]+)@i',
      '@' . $server . '/ssdcms/ipublic\.do\?u=([^"\&\? ]+)@i',
    );

    foreach ($patterns as $pattern) {
      preg_match($pattern, $embedCode, $matches);
      // @TODO: Parse is called often. Refactor so that valid ID is checked
      // when a video is added, but not every time the embedCode is parsed.
      if (isset($matches[1])) {
        return file_stream_wrapper_uri_normalize('sharestream://v/' . $matches[1]);
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

    return $file;
  }
}
