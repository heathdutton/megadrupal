<?php

/**
 * @file
 *
 * Extends the MediaReadOnlyStreamWrapper class to handle ShareStream videos.
 */

/**
 * Extend MEdiaReadOnlyStreamWrapper to create MediaShareStreamStreamWrapper.
 *
 * Create an instance like this:
 * $video = new MediaShareStreamStreamWrapper('sharestream://v/[video-code]');
 */
class MediaShareStreamStreamWrapper extends MediaReadOnlyStreamWrapper {

  static function getMimeType($uri, $mapping = NULL) {
    return 'video/sharestream';
  }

  function getLocalThumbnailPath() {

    $local_path = 'public://media-youtube/preview.png';

    if (!file_exists($local_path)) {
      $dirname = drupal_dirname($local_path);
      file_prepare_directory($dirname, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
      $image = drupal_get_path('module', 'media_sharestream') . '/preview.png';
      file_unmanaged_copy($image, $local_path, FILE_EXISTS_REPLACE);
    }

    return $local_path;
  }
}
