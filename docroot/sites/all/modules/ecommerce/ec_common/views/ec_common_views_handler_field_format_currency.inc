<?php

/**
 * @file
 * Implement handler to format currency.
 */

class ec_common_views_handler_field_format_currency extends views_handler_field {
  function render($values) {
    return format_currency($values->{$this->field_alias});
  }
}
