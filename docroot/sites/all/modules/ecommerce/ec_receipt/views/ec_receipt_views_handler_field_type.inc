<?php

/**
 * @file
 * Implement handler for receipt types.
 */

class ec_receipt_views_handler_field_type extends views_handler_field {
  function render($values) {
    return ec_receipt_get_types('name', $values->{$this->field_alias});
  }
}

