<?php

/**
 * @file
 * Provides a simple add transaction form as an area
 */

class commerce_gc_handler_area_add_transaction_form extends views_handler_area {
  function render($empty = FALSE) {
    if (user_access('create new giftcard transactions')) {
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
          
          // TODO there is a problem with this going to the "all" argument.
          $form = drupal_get_form('commerce_gc_add_transaction_action_form_simple', $coupon->coupon_id);
          return drupal_render($form);
        }
      }
    }
  }
}
