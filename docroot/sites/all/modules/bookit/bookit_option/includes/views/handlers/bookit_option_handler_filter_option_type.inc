<?php

/**
 * Filter by option type.
 */
class bookit_option_handler_filter_option_type extends views_handler_filter_in_operator {
  // Display a list of option types in the filter's options.
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Booking option type');
      $this->value_options = bookit_option_type_get_name();
    }
  }
}
