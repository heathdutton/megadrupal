<?php

/**
 * @file
 * Views handler for Tournament module
 */

class tournament_handler_filter_entity_type extends views_handler_filter_in_operator {
 function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Participant type');
      $this->value_options = match_entity_types_list();
    }
  }
}
