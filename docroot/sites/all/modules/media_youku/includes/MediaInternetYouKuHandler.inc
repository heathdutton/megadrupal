<?php

/**
 * Implementation of MediaInternetBaseHandler.
 *
 * @see hook_media_internet_providers().
 */
class MediaInternetYouKuHandler extends MediaInternetBaseHandler {

  public function get_item_code($embedCode) {
    $patterns = array(
      '@v\.youku\.com/v_show/id_([^"\&/]+)\.html@i',
      '@player\.youku\.com/player\.php/sid/([^"\&/]+)@i',
    );
    foreach ($patterns as $pattern) {
      preg_match($pattern, $embedCode, $matches);
      if (isset($matches[1])) {
        return ($matches[1]);
      }
    }
  }

  public function parse($embedCode) {
    if($this->get_item_code($embedCode)){
      return file_stream_wrapper_uri_normalize('youku://v/' . $this->get_item_code($embedCode));
    }
  }

  public function claim($embedCode) {
    if ($this->parse($embedCode)) {
      return TRUE;
    }
  }

  public function validate() {

  }

  public function save() {
    $file = $this->getFileObject();
    $link = 'http://v.youku.com/player/getPlayList/VideoIDS/' . $this->get_item_code($this->embedCode) . '/timezone/+08/version/5/source/out?password=&ran=2513&n=3';
    $content = file_get_contents($link);
    $json = @json_decode($content, true);
    if(!empty($json['data'][0]['title'])) {
      $file->filename = $json['data'][0]['title'];
    }
    file_save($file);
    return $file;
  }

  public function getFileObject() {
    //if files exist,update it.
    $uri = $this->parse($this->embedCode);
    $file = file_uri_to_object($uri);
    $existing_files = file_load_multiple(array(), array('uri' => $uri, 'filemime' => 'video/youku'));
    if (count($existing_files)) {
      $existing_file = current($existing_files);
      $file = $existing_file;
    }
    return $file;
  }
}
