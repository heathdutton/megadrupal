<?php

/**
 * @file
 * Provides the rendering of the receipt status field.
 */

class ec_receipt_views_handler_field_status extends views_handler_field {
  function render($values) {
    return ec_receipt_get_status($values->{$this->field_alias});
  }
}

