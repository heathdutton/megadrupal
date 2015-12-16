<?php
/**
 * @file
 * This file contains the information of numeric arguments
 */

class odata_handler_argument_numeric extends views_handler_argument_numeric {

  /**
   * Constructor
   */
  function construct() {
    parent::construct();
  }
  
  
  function query($group_by = FALSE) {
  $valuestring = $this->argument;
    $thisrealfield = _odata_toreal($this->real_field);
    $this->query->add_ready_contextual_filter(  urlencode($valuestring) . '+eq+' . $thisrealfield  );
  }
}
