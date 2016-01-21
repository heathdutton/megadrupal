<?php

/**
 * @file
 * Provides a transaction balance area for a giftcard coupon
 */

class commerce_gc_handler_area_balance_summary extends views_handler_area {
  function render($empty = FALSE) {
    if (!$empty || !empty($this->options['empty'])) {
      foreach ($this->view->argument as $name => $argument) {
        if (count($argument->value) == 1) {
          $value = reset($argument->value);
          
          // Find some kind of argument we can use.
          if ($argument instanceof commerce_coupon_handler_argument_coupon_code) {
            $coupon = commerce_coupon_load_by_code($value);
          }
          else if ($argument instanceof commerce_coupon_handler_argument_coupon_id) {
            $coupon = commerce_coupon_load($value);
          }
          
          if ($coupon) {
            $balance = commerce_gc_giftcard_balance($coupon->coupon_id);
            return '<div class="commerce-order-handler-area-order-total">' . t('Balance') . ': ' . commerce_currency_format($balance, commerce_default_currency()) . '</div>';
          }
        }
      }
    }
    
    return '';
  }
}
