<?php
/**
 * Field handler to provide a status name.
 */
class apply_for_role_views_handler_field_status_name extends views_handler_field {
  function render($values) {
    $value = $values->{$this->field_alias};
    return theme('apply_for_role_status', array('status' => $value));
  }
}
