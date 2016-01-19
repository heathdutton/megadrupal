<?php
/**
* @file
* Provide node nid argument handler.
*/
/**
* Argument handler to accept a person id.
*/
class league_handler_field_league_team_id extends views_handler_field {
  function render($values) {
    $value = $this->get_value($values);
    $node = node_load($value);
    return $node->title;
  }
}
