<?php

/**
 * @file
 * Views handler for Match module
 */

class team_handler_field_member_role extends views_handler_field {
  function render($values) {
    return team_get_member_roles($values->{$this->field_alias});
  }
}
