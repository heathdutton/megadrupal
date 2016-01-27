<?php
/**
* @file
* Provide node nid argument handler.
*/
/**
* Argument handler to accept a person id.
*/
class league_person_handler_field_league_person_id extends views_handler_field {
  function render($values) {
    $value = $this->get_value($values);
    $person = league_person_load($value);
    return $person->name;
  }
}
