<?php

if (!class_exists('dcBase')) {
  require_once('class.dcbase.inc');
}

/**
 * Date class definition.
 */
class dcDate extends dcBase implements dcContentInterface
{
  /***************************************************
       DATA
  ***************************************************/
    
  /***************************************************
       PUBLIC METHODS
  ***************************************************/
  
  public function __construct($subject, $options = array(), $content_data = array()) {
    parent::__construct($subject, $options, $content_data);
  }
  
  public function generate($quantity, $options = array()) {
    $output = array();

    // set date to a random value within the next 30 days
      $output['value']    = date('Y-m-d\TH:i:s', time() + 60 * 60 * 24 * mt_rand(0, 30));
      $output['value2']   = $output['value'];

    // set the timezone to site's default value
      $output['timezone'] = variable_get('date_default_timezone', 'America/Los_Angeles');

    // figure out & set the timezone offset (in seconds)
      $zones  = system_time_zones();
      if (preg_match('/([\+|-])([0-9]{1,2})([0-9]{2})$/', $zones[$output['timezone']], $matches)) {
        list($full_offset, $direction, $hours, $minutes) = $matches;
      
        $offset = (60 * 60 * (int) $hours) + (60 * $minutes);
        
        if ($direction == '-') {
          $offset *= -1;
        }
        
        $output['offset']   = $offset;
        $output['offset2']  = $offset;
      }
    
    return $output;
  }
}
