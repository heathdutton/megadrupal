<?php
/**
 * Field handler to provide a role name.
 */
class apply_for_role_views_handler_field_role_name extends views_handler_field {
  function render($values) {
    $name = db_query("SELECT name FROM {role} WHERE rid = :rid", array(':rid' => $values->{$this->field_alias}))->fetchField();
    return check_plain($name);
  }
}
