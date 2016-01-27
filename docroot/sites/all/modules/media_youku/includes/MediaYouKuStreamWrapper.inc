<?php

/**
 *  @file
 *  Create a YouKu Stream Wrapper class for the Media/Resource module.
 */

/**
 *  Create an instance like this:
 *  $youku = new ResourceYouKuStreamWrapper('youku://?v=[video-code]');
 */
class MediaYouKuStreamWrapper extends MediaReadOnlyStreamWrapper {
  protected $base_url = 'http://v.youku.com/v_show/id_';

  function getTarget($f) {
    return FALSE;
  }

  static function getMimeType($uri, $mapping = NULL) {
    return 'video/youku';
  }

  function getOriginalThumbnailPath() {
    $parts = $this->get_parameters();
    $link = 'http://v.youku.com/player/getPlayList/VideoIDS/' . urlencode($parts['v']) . '/timezone/+08/version/5/source/out?password=&ran=2513&n=3';
    $content = file_get_contents($link);
    $json = @json_decode($content, true);
    if(!empty($json['data'][0]['logo'])) {
       return $json['data'][0]['logo'];
    }
    return false;

  }

  function getLocalThumbnailPath() {
    $parts = $this->get_parameters();
    $local_path = 'public://media-youku/' . check_plain($parts['v']) . '.jpg';
    if (!file_exists($local_path)) {
      $dirname = drupal_dirname($local_path);
      file_prepare_directory($dirname, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
      @copy($this->getOriginalThumbnailPath(), $local_path);
    }
    return $local_path;
  }

  function interpolateUrl() {
    if ($parameters = $this->get_parameters()) {
      return $this->base_url . $parameters['v'] . '.html';
    }
  }
}
