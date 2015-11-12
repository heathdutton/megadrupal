<?php

if (!class_exists('dcFile')) {
  require_once('class.dcfile.inc');
}

/**
 * Image class definition.
 */
class dcImage extends dcFile implements dcContentInterface
{
  /***************************************************
       DATA
  ***************************************************/
  const mask = '/\.(jpg|jpeg|gif|png|bmp|tif)$/';
  
  protected $data = array();  
  protected static $content_db = array();
  protected $module;  

  /***************************************************
       PUBLIC METHODS
  ***************************************************/
  
  public function __construct($subject, $options = array(), $content_data = array()) {
    parent::__construct($subject, $options, $content_data);

    // find all files and categorize them by folder name
      if (empty($this->content_db) && !empty($content_data['images'])) {
        foreach ($content_data['images'] as $category => $paths) {
          // handle single-string cases
          if (!is_array($paths)) {
            $paths = array($paths);
          }
          
          foreach($paths as $path) {
            foreach (file_scan_directory('./' . $path, self::mask) as $file) {
              $fileparts = explode('/', $file->uri);
              $this->content_db[$category][] = $file->uri;
            }
          }
        }
      }
    
    // determine which images should be displayed for the current subject
      if (!empty($this->subject) && !empty($this->content_db[$this->subject])) {
        $this->data = $this->content_db[$this->subject];
      }
      // if the requested category is not found, pick a random image from existing content
      else {
        foreach ($this->content_db as $category) {
          $this->data = array_merge($this->data, $category);
        }
      }
    
    $this->size = sizeof($this->data);
  }
  
  /***************************************************
       PRIVATE METHODS
  ***************************************************/
}
