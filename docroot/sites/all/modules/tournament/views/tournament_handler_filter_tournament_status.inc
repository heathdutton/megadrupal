<?php

/**
 * @file
 * Views handler for Tournament module
 */

class tournament_handler_filter_tournament_status extends views_handler_filter_in_operator {
 function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Tournament status');
      $this->value_options = _tournament_get_status();
    }
  }
}
