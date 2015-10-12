<?php

if (!interface_exists('dcContentInterface')) {
  require_once('interface.dccontent.inc');
}

/**
 * Base class definition.
 */
class dcBase implements dcContentInterface
{
  /***************************************************
       PUBLIC METHODS
  ***************************************************/
  
  public function __construct($subject, $options = array(), $content_data = array()) {
    $this->subject = $subject;
  }
  
  public function generate($quantity, $options = array()) {
    return FALSE;
  }
}
