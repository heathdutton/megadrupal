<?php

/**
 * @file views_s3_handler_field.inc
 * Basic S3 field handler.
 */

/**
 * Field handler to provide simple renderer.
 */
class views_s3_handler_field extends views_handler_field {
  
  function render($values) {
    $value = $this->get_value($values);
    return $this->sanitize_value($value);
  }

  /**
   * Called to add the field to a query.
   */
  function query() {
    $this->field_alias = $this->real_field;
  }

}
