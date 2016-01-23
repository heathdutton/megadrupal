<?php 
/**
 *  @file
 *  Adds a field as date
 */

class  odata_handler_field_complex_date extends views_handler_field_date {

  function construct() {
    parent::construct();
  }
  
  function query() {
    $name = explode('-', $this->real_field);
  
    $this->query->select_fields[] = $name[0];
  }

  function render($values) {
    $name = explode('-', $this->real_field);
    $value = $values->{$name[0]}[$name[1]];
    $value = intval( intval(drupal_substr($value, 6))/1000);
    $values->{$this->real_field} = $value;
    $values->{$this->field_alias} = $value;
    //If you uncomment these lines, then the output dates will be displayed at UTC timezone.
    //$beforetimezone = date_default_timezone_get();
    //date_default_timezone_set( 'UTC' );
    $ret = parent::render($values);
    //date_default_timezone_set( $beforetimezone );
    return $ret;
  }//
}
