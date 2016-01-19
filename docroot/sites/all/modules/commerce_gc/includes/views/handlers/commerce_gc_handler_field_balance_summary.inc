<?php

/**
 * @file
 * Provides a balance summary field for giftcards
 */

class commerce_gc_handler_field_balance_summary extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['coupon_id'] = 'coupon_id';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }
  
  function render($values) {
    $coupon = commerce_coupon_load($this->get_value($values, 'coupon_id'));
    if ($coupon) {
      $balance = commerce_gc_giftcard_balance($coupon->coupon_id);
      
      $output = t('Balance: @balance', array('@balance' => commerce_currency_format($balance, commerce_default_currency()))) . '<br/>' .
        l(t('Transactions'), 'giftcards/' . $coupon->coupon_id . '/transactions');
      
      return $output;
    }
  }
}
