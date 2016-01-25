<?php

/**
 * @file
 * Provide translation of allocation type to human readable format.
 */

class ec_receipt_views_handler_field_allocation_type extends views_handler_field {
  function render($values) {
    return ec_receipt_get_atypes('name', $values->{$this->field_alias});
  }
}

