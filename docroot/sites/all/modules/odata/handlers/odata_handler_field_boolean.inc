<?php 
/**
 *  @file
 * Adds a field as boolean
 */
class  odata_handler_field_boolean extends views_handler_field_boolean {
  
  function construct() {
    parent::construct();
  }
  
  function query() {
    $this->field_alias = $this->real_field;
    $this->query->select_fields[] = $this->real_field;
  }

}
