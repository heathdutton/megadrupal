<?php

/**
 * Implementation of MediaInternetBaseHandler.
 *
 * @see hook_media_internet_providers().
 */

class MediaInternetTuDouHandler extends MediaInternetBaseHandler {

  public function get_item_code($embedCode) {
    $patterns = array(
      '@tudou\.com/programs/view/([^"\&/]+)@i',
      '@tudou\.com/v/([^"\&/]+)@i',
      '@tudou\.com/l/([^"\&/]+)@i',
      '@tudou\.com/a/([^"\&/]+)@i',
      '@tudou\.com/albumplay.*/([^"\&/]+)\.html$@i',
      '@tudou\.com/listplay.*/([^"\&/]+)\.html$@i',
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
      return file_stream_wrapper_uri_normalize('tudou://v/' . $this->get_item_code($embedCode));
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
    $link = 'http://api.tudou.com/v3/gw?method=item.info.get&appKey=myKey&format=json&itemCodes=' . $this->get_item_code($this->embedCode);
    $content = file_get_contents($link);
    $json = @json_decode($content, true);
    if(!empty($json['multiResult']['results'][0]['title'])) {
      $file->filename = $json['multiResult']['results'][0]['title'];
    }
    file_save($file);
    return $file;
  }

  public function getFileObject() {
    $uri = $this->parse($this->embedCode);
    $file = file_uri_to_object($uri);
    $existing_files = file_load_multiple(array(), array('uri' => $uri, 'filemime' => 'video/tudou'));
    if (count($existing_files)) {
      $existing_file = current($existing_files);
      $file = $existing_file;
    }    
    return $file;
  }
}
