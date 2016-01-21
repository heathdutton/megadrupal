<?php

/**
 * @file
 * Views handler: Product price field.
 */

/**
 * Returns a formatted price value to display in the View.
 */
class uc_multiprice_handler_field_money_amount extends uc_product_handler_field_price {

  /**
   * Overrides views_handler_field::render().
   */
  function render($values) {
    $order = uc_order_load($values->order_id);
    $currency_sign = uc_multiprice_get_currency_sign($order->currency);
    if ($this->options['format'] == 'numeric') {
      return parent::render($values);
    }

    if ($this->options['format'] == 'uc_price') {
      return uc_currency_format($values->{$this->field_alias}, $currency_sign);
    }
  }
}
