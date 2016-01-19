<?php

/**
 * Implementation of MediaInternetBaseHandler.
 *
 * @see hook_media_internet_providers().
 */
class MediaInternetYandexHandler extends MediaInternetBaseHandler {
  public function parse($embedCode) {
    if (preg_match('#video.yandex.ru/users/[^/]+/view/\d+#i', $embedCode)) {
      $page = file_get_contents($embedCode);
      if (preg_match('#http://static.video.yandex.ru/lite/([^/]+)/([^/]+)/#i', $page, $matches)) {
        return file_stream_wrapper_uri_normalize('yandex://u/' . $matches[1] . '/v/' . $matches[2]);
      }
    }
  }

  public function claim($embedCode) {
    if ($this->parse($embedCode)) {
      return TRUE;
    }
  }

  public function validate() {
    // @todo Media module currently fails when two files try to have the same
    //   URI, so catch that in the validation step. Some day, it would be nice
    //   to allow it, however. See http://drupal.org/node/952422.
    $uri = $this->parse($this->embedCode);
    $existing_files = file_load_multiple(array(), array('uri' => $uri));
    if (count($existing_files)) {
      throw new MediaInternetValidationException(t('You have entered a URL for a video that is already in your library.'));
    }
  }

  public function save() {
    $file = $this->getFileObject();
    file_save($file);
    return $file;
  }

  public function getFileObject() {
    $uri = $this->parse($this->embedCode);
    //@todo: this is terribly broken in some ways because the function is really
    // made for local files which are 'real'
    return file_uri_to_object($uri);
  }
}
