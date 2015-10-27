<?php

/**
 * @file views_s3_handler_field.inc
 * Basic S3 field handler.
 */

/**
 * Field handler to provide simple renderer.
 */
class views_s3_handler_field_timestamp extends views_handler_field_date {
  
  function render($values) {
    $values->{$this->field_alias} = strtotime($this->get_value($values));    
    return parent::render($values);
  }

  /**
   * Called to add the field to a query.
   */
  function query() {
    $this->field_alias = $this->real_field;
  }

}
