<?php

/**
 * @file views_s3_handler_field.inc
 * Basic S3 field handler.
 */

/**
 * Field handler to provide simple renderer.
 */
class views_s3_handler_field_nested extends views_s3_handler_field {
  
  function render($values) {
    $values = (array)$values;
    foreach ($values as $key => $value) {
      if (is_array($value)) {
        foreach ($value as $nested_key => $nested_value) {
          $values[$key . $nested_key] = $nested_value;
        }
        unset($values[$key]);
      }
    }
    $values = (object)$values;
    $value = $this->get_value($values);
    return $this->sanitize_value($value);
  }

}
