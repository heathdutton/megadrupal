<?php
/**
* @file
* Provide node nid argument handler.
*/
/**
* Argument handler to accept a person id.
*/
class league_game_handler_field_league_game_id extends views_handler_field {
  function render($values) {
    $value = $this->get_value($values);
    return league_game_get_title($value);
  }
}
