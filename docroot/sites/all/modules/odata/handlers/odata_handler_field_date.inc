<?php 
/**
 *  @file
 *  Adds a field as date
 */

class  odata_handler_field_date extends views_handler_field_date {

  function construct() {
    parent::construct();
  }
  
  function query() {
    $this->field_alias = $this->real_field;
    $this->query->select_fields[] = $this->real_field;
  }

  function render($values) {
    $values->{$this->real_field} = intval( intval(drupal_substr($values->{$this->real_field}, 6))/1000);
    //If you uncomment these lines, then the output dates will be displayed at UTC timezone.
    //$beforetimezone = date_default_timezone_get();
    //date_default_timezone_set( 'UTC' );
    $ret = parent::render($values);
    //date_default_timezone_set( $beforetimezone );
    return $ret;
  }//
}
