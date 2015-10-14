<?php
/**
 * @file
 * Displays the granted amount of a coupon based in each coupon type conditions.
 */

class commerce_coupon_handler_field_coupon_granted_amount extends views_handler_field {

  function construct() {
    parent::construct();

    $this->additional_fields['coupon_id'] = 'coupon_id';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $amount = '';
    // Get the coupon & order from the entities returned by the view.
    $coupons = $this->view->query->get_result_entities(array($values));
    $coupon = reset($coupons[1]);
    list(, $orders) = $this->query->get_result_entities(array($values), 'commerce_coupon_order_reference_commerce_coupon');
    $order = reset($orders);

    drupal_alter('commerce_coupon_granted_amount', $amount, $coupon, $order);
    return $amount;
  }

}
