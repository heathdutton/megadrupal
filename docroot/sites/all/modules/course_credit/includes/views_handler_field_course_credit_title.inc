<?php

/**
 * Field handler for course credit title.
 */
class views_handler_field_course_credit_title extends views_handler_field {

  function render($values) {
    $value = $values->{$this->field_alias};
    return filter_xss($value);
  }

}
