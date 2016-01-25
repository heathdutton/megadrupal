<?php

/**
 * @file
 * Delete coupon link field handler.
 */

/**
 * Delete coupon link field handler.
 */
class commerce_coupon_handler_field_coupon_delete extends commerce_coupon_handler_field_coupon_link {

  /**
   * Overrides parent::render($values).
   */
  function render($values) {
    $coupon_id = $this->get_value($values, 'coupon_id');
    if (!$this->check_access($coupon_id, 'delete')) {
      return;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : t('Delete');

    return l($text, 'admin/commerce/coupons/' . $coupon_id . '/delete', array('query' => drupal_get_destination()));
  }

}
