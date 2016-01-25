<?php

/**
 * @file
 * Handles the filtering by allocation types.
 */

class ec_receipt_views_handler_filter_allocation_type extends views_handler_filter_in_operator {
  function get_value_options() {
    $this->value_options = ec_receipt_get_atypes('names');
  }
}
