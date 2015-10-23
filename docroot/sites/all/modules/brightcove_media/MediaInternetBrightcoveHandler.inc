<?php
/**
 * @file
 * Internet handler for Brightcove videos.
 */

class MediaInternetBrightcoveHandler extends MediaInternetBaseHandler {
  public function parse($embedCode) {
    // special handling for the shortened urls
    if (preg_match('#^http://bcove.me/[a-z0-9]+$#i', $embedCode)) {
      $resp = drupal_http_request($embedCode);
      if ($resp) {
        $full_url = $resp->redirect_url;
        $matches = NULL;
        preg_match('#&bctid=([0-9]+)#i', $full_url, $matches);
        if (isset($matches[1])) {
          return file_stream_wrapper_uri_normalize(
            "brightcove://{$matches[1]}");
        }
      }
    }

    // using regexes are right here, because brightcove provides badly
    // formatted html files
    $regexes = array(
      '#^([0-9]+)$#i',
      '#name="@videoPlayer" value="([0-9]+)"#i',
      '#@videoPlayer=([0-9]+)&#i',
    );

    foreach ($regexes as $regex) {
      $matches = NULL;
      preg_match($regex, $embedCode, $matches);
      if (isset($matches[1])) {
        return file_stream_wrapper_uri_normalize("brightcove://{$matches[1]}");
      }
    }

    return NULL;
  }

  public function claim($embedCode) {
    return (bool) $this->parse($embedCode);
  }

  public function validate() {
    $uri = $this->parse($this->embedCode);
    $existing_files = file_load_multiple(array(), array('uri' => $uri));
    if (count($existing_files)) {
      throw new MediaInternetValidationException(t(
          'You have entered a URL for a video that is already in your library.'
        ));
    }
  }

  public function save() {
    $file = $this->getFileObject();
    file_save($file);
    return $file;
  }

  /**
   * Returns a file object which can be used for validation
   *
   * @return StdClass
   */
  public function getFileObject() {
    $uri = $this->parse($this->embedCode);
    return brightcove_media_file_uri_to_object($uri);
  }

}
