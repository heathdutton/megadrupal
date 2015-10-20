<?php

/**
 *  @file
 *  Create a Yandex Video Stream Wrapper class for the Media/Resource module.
 */

/**
 *  Create an instance like this:
 *  $wrapper = new ResourceYandexStreamWrapper('yandex://u/[user-name]/v/[video-code]');
 */
class MediaYandexStreamWrapper extends MediaReadOnlyStreamWrapper {
  function getTarget($f) {
    return FALSE;
  }

  static function getMimeType($uri, $mapping = NULL) {
    return 'video/yandex';
  }

  function getOriginalThumbnailPath() {
    $parts = $this->get_parameters();
    return 'http://static.video.yandex.ru/get/' . check_plain($parts['u']) . '/' . check_plain($parts['v']) . '/1.320x240.jpg';
  }

  function getLocalThumbnailPath() {
    $parts = $this->get_parameters();
    $local_path = 'public://media-yandex/' . check_plain($parts['v']) . '.jpg';
    if (!file_exists($local_path)) {
      $dirname = drupal_dirname($local_path);
      file_prepare_directory($dirname, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
      @copy($this->getOriginalThumbnailPath(), $local_path);
    }
    return $local_path;
  }
}

