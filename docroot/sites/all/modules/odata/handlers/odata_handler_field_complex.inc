<?php
/**
 *  @file
 * Adds a field as a general field. Markup fields use this handler.
 */

/**
 * Handler for a subject.
 */
class odata_handler_field_complex extends views_handler_field {

  function construct() {
    parent::construct();
  }

  /**
  * Called to add the field to a query.
  */
  function query() {
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
