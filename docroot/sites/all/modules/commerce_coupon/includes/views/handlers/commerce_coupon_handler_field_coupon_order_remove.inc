<?php
/**
 * @file
 * View handler for removing a commerce coupon line item from a given order.
 */

class commerce_coupon_handler_field_coupon_order_remove extends views_handler_field {
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
    list(, $orders) = $this->query->get_result_entities(array($values), 'commerce_coupon_order_reference_commerce_coupon');
    $order = reset($orders);


    // If there's an ajax form in the page, get the right destination url.
    if (isset($_SERVER['HTTP_X_REQUESTED_WITH']) && $_SERVER['HTTP_X_REQUESTED_WITH'] == 'XMLHttpRequest')  {
      $dest = str_replace($GLOBALS['base_url'] . base_path(), '', $_SERVER['HTTP_REFERER']);
    }
    else {
      $dest = current_path();
    }

    return l(
      t('Remove'),
      'commerce/coupons/order/remove/' . $coupon_id . '/' . $order->order_id,
      array('query' => array('destination' => $dest, 'token' => drupal_get_token('commerce_coupon_remove_checkout:' . $coupon_id . ':' . $order->order_id)))
    );
  }
}
