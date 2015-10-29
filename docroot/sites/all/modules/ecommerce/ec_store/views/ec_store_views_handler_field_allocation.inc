<?php

/**
 * @file
 * Provide output of allocation status description.
 */

class ec_store_views_handler_field_allocation extends views_handler_field {
  function render($values) {
    return ec_store_transaction_get_allocation($values->{$this->field_alias});
  }
}
