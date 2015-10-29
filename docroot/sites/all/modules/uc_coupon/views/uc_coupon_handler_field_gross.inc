<?php
// $Id$

/**
 * @file
 * Coupon order total field handler
 */
class uc_coupon_handler_field_gross extends uc_views_handler_field_money_amount {
  function query() {
    $this->ensure_my_table();
    $this->query->ensure_table('uc_orders');
    $this->field_alias = $this->query->add_field(NULL, '(uc_orders.order_total + uc_coupons_orders.value)',
      $this->table_alias .'_'. $this->field
    );
  }
}

