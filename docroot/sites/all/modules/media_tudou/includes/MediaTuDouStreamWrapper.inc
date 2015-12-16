<?php

/**
 *  @file
 *  Create a TuDou Stream Wrapper class for the Media/Resource module.
 */

/**
 *  Create an instance like this:
 *  $tudou = new ResourceTuDouStreamWrapper('tudou://?v=[video-code]');
 */
class MediaTuDouStreamWrapper extends MediaReadOnlyStreamWrapper {
  protected $base_url = 'http://www.tudou.com/programs/view/';

  function getTarget($f) {
    return FALSE;
  }

  static function getMimeType($uri, $mapping = NULL) {
    return 'video/tudou';
  }

  function getOriginalThumbnailPath() {
    $parts = $this->get_parameters();
    if(file_exists($local_path = 'public://media-tudou/' . check_plain($parts['v']) . '.jpg')) {
      return $local_path;
    }
    $link = 'http://api.tudou.com/v3/gw?method=item.info.get&appKey=myKey&format=json&itemCodes=' . check_plain($parts['v']) ;
    $content = file_get_contents($link);
    $json = @json_decode($content, true);
     if(!empty($json['multiResult']['results'][0]['picUrl'])) {
       return $json['multiResult']['results'][0]['picUrl'];
      }
    return false;
  }

  function getLocalThumbnailPath() {
    $parts = $this->get_parameters();
    $local_path = 'public://media-tudou/' . check_plain($parts['v']) . '.jpg';
    if (!file_exists($local_path)) {
      $dirname = drupal_dirname($local_path);
      file_prepare_directory($dirname, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
      @copy($this->getOriginalThumbnailPath(), $local_path);
    }
    return $local_path;
  }
}
