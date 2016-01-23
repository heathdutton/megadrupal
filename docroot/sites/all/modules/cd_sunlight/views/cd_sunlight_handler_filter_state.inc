<?php

/**
 * Filter handler for state.  This is very basic, but cd_civicrm can override it.
 */
class cd_sunlight_handler_filter_state extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $query = db_select('cd_sunlight_legislators', 'l');
      $query->addField('l', 'state');
      $query->addField('l', 'state');
      $this->value_options = $query->groupBy('state')
        ->orderBy('state')
        ->execute()
        ->fetchAllKeyed();
    }
  }
}
