<?php

/**
 * Filter handler for party.
 */
class cd_sunlight_handler_filter_party extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_options = array(
        'D' => t('Democrat'),
        'R' => t('Republican'),
        'I' => t('Independent'),
      );
    }
  }
}
