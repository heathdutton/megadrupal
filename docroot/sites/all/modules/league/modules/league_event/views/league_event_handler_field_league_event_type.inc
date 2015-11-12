<?php
/**
* @file
* Provide node nid argument handler.
*/
/**
* Argument handler to accept a person id.
*/
class league_event_handler_field_league_event_type extends views_handler_field {
  function render($values) {
    $value = $this->get_value($values);
    $status_map = league_event_get_event_types();
    return $status_map[$value];
  }
}
