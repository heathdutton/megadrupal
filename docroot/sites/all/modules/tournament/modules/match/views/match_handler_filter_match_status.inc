<?php

/**
 * @file
 * Views handler for Match module
 */

class match_handler_filter_match_status extends views_handler_filter_in_operator {
 function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Match status');
      $this->value_options = match_status_list();
    }
  }
}
