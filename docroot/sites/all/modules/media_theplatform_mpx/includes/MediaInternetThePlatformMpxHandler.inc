<?php
/**
 * @file
 * Contains MediaInternetThePlatformMpxHandler.
 */

class MediaInternetThePlatformMpxHandler extends MediaInternetBaseHandler {

  /**
   * Return TRUE if $embed_code is for a mpx file.
   */
  public function parse($embed_code) {

    // Valid format: mpx://[anything here].
    preg_match('@^mpx://([^/]+)@i', $embed_code, $matches);

    $host = isset($matches[1]);
    if ($host) {
      return file_stream_wrapper_uri_normalize($embed_code);
    }
    else {
      return FALSE;
    }
  }

  /**
   * Mpx handler is import-only and thus no ability to 'claim' entered URLs.
   */
  public function claim($embed_code) {
    if ($this->parse($embed_code)) {
      return TRUE;
    }
  }

  /**
   * There is no need to validate - all videos are ingested by thePlatform API.
   */
  public function validate() {
  }

  /**
   * Save file object.
   */
  public function save($account = NULL, $title = NULL) {
    $file = $this->getFileObject($account);

    if ($title != NULL) {
      $file->filename = $title;
    }

    media_theplatform_mpx_debug($file, "Saving file object in MediaInternetThePlatformMpxHandler::save()");
    file_save($file);
    return $file;
  }

  /**
   * Get file type.
   */
  public function getFileType($file, $account = NULL) {
    $file_type = 'video';
    drupal_alter('media_theplatform_mpx_file_type', $file_type, $file, $account);
    return $file_type;
  }

  /**
   * Return mpx File Object with type video.
   */
  public function getFileObject($account = NULL) {
    $uri = $this->parse($this->embedCode);
    $file = file_uri_to_object($uri);
    $file->type = $this->getFileType($file, $account);
    return $file;
  }

  /**
   * Not valid for ThePlatform.
   */
  public function getMRSS() {
  }

  /**
   * Not valid for ThePlatform.
   */
  public function getOEmbed() {
  }
}
