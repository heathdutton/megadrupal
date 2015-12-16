<?php 
/**
 * @file
 * Adds fields as numbers
 */
class  odata_handler_field_complex_numeric extends views_handler_field_numeric {

  function construct() {
    parent::construct();
  }

  function query() {
    $this->field_alias = $this->real_field;
    $name = explode('-', $this->real_field);

    $this->query->select_fields[] = $name[0];
  }
  
  function render($values) {
    $name = explode('-', $this->real_field);
    $value = $values->{$name[0]}[$name[1]];
    $values->{$this->real_field} = $value;
    $values->{$this->field_alias} = $value;
    return  parent::render($values) ;
  }
  
}
