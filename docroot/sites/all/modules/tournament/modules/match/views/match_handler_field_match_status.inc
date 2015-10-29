<?php

/**
 * @file
 * Views handler for Match module
 */

class match_handler_field_match_status extends views_handler_field {
  function render($values) {
    return match_status_list($values->{$this->field_alias});
  }
}
