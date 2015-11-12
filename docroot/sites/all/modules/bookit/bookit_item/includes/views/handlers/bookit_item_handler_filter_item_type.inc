<?php

/**
 * Filter by booking type.
 */
class bookit_item_handler_filter_item_type extends views_handler_filter_in_operator {
  // Display a list of booking types in the filter's options.
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Booking type');
      $this->value_options = bookit_item_type_get_name();
    }
  }
}
