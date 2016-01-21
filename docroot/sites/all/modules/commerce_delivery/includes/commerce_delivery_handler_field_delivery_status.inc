<?php

/**
 * @file
 * Field handler to select a delivery status.
 */
class commerce_delivery_handler_field_delivery_status extends views_handler_field {
  function render($values) {
    $value = commerce_delivery_status_get_title($values->{$this->field_alias});
    // Only attempt to render a valid order status.
      return $value;
  }
}
