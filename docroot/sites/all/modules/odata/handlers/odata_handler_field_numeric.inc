<?php 
/**
 * @file
 * Adds fields as numbers
 */
class  odata_handler_field_numeric extends views_handler_field_numeric {

  function construct() {
    parent::construct();
  }

  function query() {
    $this->field_alias = $this->real_field;
    $this->query->select_fields[] = $this->real_field;
  }
}
