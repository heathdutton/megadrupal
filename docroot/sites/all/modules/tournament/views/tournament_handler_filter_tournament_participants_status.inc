<?php

/**
 * @file
 * Views handler for Tournament module
 */

class tournament_handler_filter_tournament_participants_status extends views_handler_filter_in_operator {
 function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Participant status');
      $this->value_options = array(0 => t('Not active'), 1 => t('Active'));
    }
  }
}
