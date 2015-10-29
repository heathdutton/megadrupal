<?php

/**
 * @file
 * Implement handler for filter by transaction type.
 */

class ec_store_views_handler_filter_txn_type extends views_handler_filter_in_operator {
  function get_value_options() {
    $this->value_options = module_invoke_all('ec_transaction_types');
  }
}
