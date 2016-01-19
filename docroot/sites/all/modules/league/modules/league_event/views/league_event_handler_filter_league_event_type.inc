<?php
/**
* @file
* Provide node nid argument handler.
*/
/**
* Argument handler to accept a person id.
*/
class league_event_handler_filter_league_event_type extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Event types');
      $types = league_event_get_event_types();
      $this->value_options = $types;
    }
  }
}
