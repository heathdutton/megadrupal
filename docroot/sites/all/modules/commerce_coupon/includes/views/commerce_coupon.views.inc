<?php

/**
 * @file
 * Coupon views extra data.
 */

/**
 * Alter the views data to enable some additional features for coupons in views.
 */
function commerce_coupon_views_data_alter(&$data) {
  if (isset($data['commerce_coupon'])) {
    $data['commerce_coupon']['operations'] = array(
      'field' => array(
        'title' => t('Operations'),
        'help' => t('Display all the available operations links for the coupon.'),
        'handler' => 'commerce_coupon_handler_field_coupon_operations',
      ),
    );
    $data['commerce_coupon']['granted_amount'] = array(
      'field' => array(
        'title' => t('Granted amount'),
        'help' => t('Display the granted amount of the coupon.'),
        'handler' => 'commerce_coupon_handler_field_coupon_granted_amount',
      ),
    );
    // Expose the number of times the coupon has been used.
    $data['commerce_coupon']['times_used'] = array(
      'title' => t('Times used'),
      'help' => t('Number of times that the coupon has been used.'),
      'field' => array(
        'handler' => 'commerce_coupon_handler_field_times_used',
        'click sortable' => TRUE,
      ),
    );
    $data['commerce_coupon']['remove_from_order'] = array(
      'field' => array(
        'title' => t('Remove from Order'),
        'help' => t('Remove coupon from the order.'),
        'handler' => 'commerce_coupon_handler_field_coupon_order_remove',
      ),
    );
  }
}
