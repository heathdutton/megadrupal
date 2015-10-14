<?php

/**
 * Field handler to display the times a coupon has been used.
 */
class commerce_coupon_handler_field_times_used extends commerce_order_handler_field_order {
  function construct() {
    parent::construct();
    $this->additional_fields['coupon_id'] = 'coupon_id';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $coupon_id = $this->get_value($values, 'coupon_id');
    return commerce_coupon_get_number_of_uses($coupon_id);
  }
}
