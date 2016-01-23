<?php 
/**
 *  @file
 * Adds a field as boolean
 */
class  odata_handler_field_complex_boolean extends views_handler_field_boolean {
  
  
  function construct() {
    parent::construct();
  }

  /**
  * Called to add the field to a query.
  */
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
