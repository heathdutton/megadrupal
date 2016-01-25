<?php

/**
 * @file
 * Implements display handler for product types.
 */

class ec_product_handler_field_ptype extends views_handler_field {
  function render($values) {
    if (isset($values->{$this->field_alias})) {
      $value = ec_product_get_types('name', $values->{$this->field_alias});
      return check_plain($value);
    }
  }
}

