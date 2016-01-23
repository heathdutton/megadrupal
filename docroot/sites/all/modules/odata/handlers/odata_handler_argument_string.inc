<?php
/**
 * @file
 * This file contains the information of string arguments
 */

class odata_handler_argument_string extends views_handler_argument_string {

  /**
   * Constructor
   */
  function construct() {
    parent::construct();
  }
  
  
  function query($group_by = FALSE) {
    //echo '<pre>';print_r($this);exit();
  $valuestring = $this->argument;
    $thisrealfield = _odata_toreal($this->real_field);
    $this->query->add_ready_contextual_filter(  '\'' . urlencode($valuestring) . '\'' . '+eq+' . $thisrealfield  );
  }
}
