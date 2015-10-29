<?php

/**
 * @file
 * Handles the filtering by receipt types.
 */

class ec_receipt_views_handler_filter_type extends views_handler_filter_in_operator {
  function get_value_options() {
    $this->value_options = ec_receipt_get_types('names');
  }
}
