<?php

if (!class_exists('dcBase')) {
  require_once('class.dcbase.inc');
}

/**
 * Select class definition.
 */
class dcSelect extends dcBase implements dcContentInterface
{
  /***************************************************
       DATA
  ***************************************************/

  private static $data = array();
  
  private $size = 0;

  private $value_name = 'value';

  /***************************************************
       PUBLIC METHODS
  ***************************************************/
  
  public function __construct($subject, $options = array(), $content_data = array()) {
    parent::__construct($subject, $options, $content_data);
    
    $this->data = $options['options'] ? $options['options'] : array();
    
    $this->size = sizeof($this->data);
    
    $this->value_name = $options['value_name'];
  }
  
  public function generate($quantity, $options = array()) {
    $pick = mt_rand(0, $this->size - 1);
    
    $item = array(
      $this->value_name => $this->data[$pick],  
    );
    
    unset($this->data[$pick]);
    
    return $item;
  }
  
  /***************************************************
       PRIVATE METHODS
  ***************************************************/
}
