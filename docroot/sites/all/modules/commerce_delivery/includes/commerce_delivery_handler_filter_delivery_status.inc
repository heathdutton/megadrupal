<?php

/**
 * @file
 * Field handler to select a delivery status.
 */
class commerce_delivery_handler_filter_delivery_status extends views_handler_filter_in_operator {
  function get_value_options() {
    $this->value_options = commerce_delivery_get_status();
  }
}
