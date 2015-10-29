<?php
// $Id$

/**
 * @file
 * Filter based on coupon order gross (total+value)
 */
class uc_coupon_handler_filter_gross extends views_handler_filter_numeric {

  function query() {
    $this->ensure_my_table();
    $uco = $this->query->ensure_table('uc_orders');
    $uc  = $this->table_alias;
    $field     = "($uco.order_total + $uc.value)";

    $info = $this->operators();
    if (!empty($info[$this->operator]['method'])) {
      $this->{$info[$this->operator]['method']}($field);
    }
  }

}

