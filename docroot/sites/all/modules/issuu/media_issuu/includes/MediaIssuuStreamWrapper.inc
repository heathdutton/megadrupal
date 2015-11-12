<?php
/**
 * @file media_issuu/includes/MediaIssuuStreamWrapper.inc
 * Create a Issuu Stream Wrapper class for the Media/Resource module.
 */

/**
 * Stream Wrapper class for Issuu.
 * Create an instance like this:
 * $issuu = new ResourceIssuuStreamWrapper('issuu://document/[issuu-name]');
 */
class MediaIssuuStreamWrapper extends MediaReadOnlyStreamWrapper {

  /**
   * Get MimeType.
   */
  public static function getMimeType($uri, $mapping = NULL) {
    return 'document/issuu';
  }
  
  function getTarget($f) {
    return FALSE;
  }

  /**
   * Get original thumbnail path.
   */
  public function getOriginalThumbnailPath() {
    $response = $response_with_id = drupal_http_request("http://issuu.com/" . substr($this->getURI(), 8));
    if (!isset($response->error)) {
      // get <meta property="og:title" content="Dubbelpunt oktober2013 FC de leidingsploeg"> form $response->data
      $doc = new DOMDocument();
      libxml_use_internal_errors(true);
      $doc->loadHTML($response->data);
      libxml_clear_errors();    
      foreach( $doc->getElementsByTagName('link') as $link ) { 
        if($link->getAttribute('rel') == 'image_src'){
          $thumbnail_url = $link->getAttribute('href');
          return $thumbnail_url;
        }
      }
    }
  }
  /**
   * Get local thumbnail path.
   */
  public function getLocalThumbnailPath() {
    $local_path = 'public://media-issuu/' . substr($this->getURI(), 8) . '.jpg';
    if (!file_exists($local_path)) {
      $dirname = drupal_dirname($local_path);
      file_prepare_directory($dirname, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
      @copy($this->getOriginalThumbnailPath(), $local_path);
    }
    return $local_path;
  }
}
