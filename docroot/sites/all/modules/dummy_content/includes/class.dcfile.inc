<?php

if (!class_exists('dcBase')) {
  require_once('class.dcbase.inc');
}

/**
 * File class definition.
 */
class dcFile extends dcBase implements dcContentInterface
{
  /***************************************************
       DATA
  ***************************************************/
  
  protected $module;  

  /***************************************************
       PUBLIC METHODS
  ***************************************************/
  
  public function __construct($subject, $options = array(), $content_data = array()) {
    parent::__construct($subject, $options, $content_data);
    
    $this->module = (!empty($options['module'])) ? $options['module'] : 'unknown';
    
    // child classes will set the type property and handle their own
    // file lookups
    if (!isset($options['type'])) {
      // make the temporary file
        if (!class_exists('dcText')) {
          require_once('class.dctext.inc');
        }
        
        if ($path = drupal_tempnam('temporary://', 'dummy_content_')) {
          $text = new dcText($this->subject);  
            
          $destination = $path . '.txt';
          file_unmanaged_move($path, $destination);
      
          $fh = fopen($destination, 'w');
          fwrite($fh, $text->generate(mt_rand(1, 10), array('type' => 'paragraph')));
          fclose($fh);
      
          $this->data[] = $destination;
          
          $this->size = sizeof($this->data);
        }
    }
  }
  
  public function generate($quantity, $options = array()) {
    if ($this->size) {  
      global $user;
  
      $uri_index = array_rand($this->data);
      $uri = $this->data[$uri_index];
      $fileparts = explode('/', $uri);
  
      $file = new stdClass();
      $file->uid = $user->uid;
      $file->uri = $uri;
      $file->filemime = file_get_mimetype($file->uri);
      $file->status = 1;
      $file->display = 1;
      $file->dummy_content = array(
        'bundle'  => 'file',
        'module'  => $this->module,
      );
      
      $file = file_copy($file, file_default_scheme() . '://' . array_pop($fileparts));
      
      if (!empty($options['delete']) && $options['delete']) {
        file_unmanaged_delete($uri);
        unset($this->data[$uri_index]);
      }
      
      return $file;
    }
    else {
      return FALSE;
    }
  }
  
  /***************************************************
       PRIVATE METHODS
  ***************************************************/
}
