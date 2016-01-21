<?php
/**
 * @file
 * This file contains the information of boolean arguments
 */

class odata_handler_argument_boolean extends views_handler_argument_string {

  /**
   * Constructor
   */
  function construct() {
    parent::construct();
  }
  
  
  function query($group_by = FALSE) {
  $valuestring = $this->argument;
    $thisrealfield = _odata_toreal($this->real_field);
    $this->query->add_ready_contextual_filter( ((drupal_strtolower($valuestring) == 'true') ? '' : 'not+' ) . $thisrealfield  );
  }
}
