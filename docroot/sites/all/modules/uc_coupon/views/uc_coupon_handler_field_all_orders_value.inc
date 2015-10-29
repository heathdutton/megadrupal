<?php
// $Id$

/**
 * @file
 * Coupon order total field handler
 */
class uc_coupon_handler_field_all_orders_value extends uc_coupon_handler_field_all_orders_total {
  /**
   * Here we add the aggregate field that will sum the value.
   */
  function query() {
    $this->ensure_my_table();
    $uco = $this->query->ensure_table('uc_coupons_orders');
    $uo = $this->query->ensure_table('uc_orders');
    $this->add_additional_fields();
    if (empty($this->options['statuses'])) {
      // If no status specified, then we show all.
      $in_status = "1";
    }
    else {
      // An array of statuses was specified.
      $in_status =  "$uo.order_status IN('" . implode("', '", $this->options['statuses']) . "')";
    }
    $this->field_alias = $this->query->add_field(
      NULL,
      "CASE WHEN $in_status THEN $uco.value ELSE 0 END",
      $this->table_alias . '_' . $this->field . '_' . implode('_', $this->options['statuses']),
      array('function' => 'sum')
    );
  }
}

