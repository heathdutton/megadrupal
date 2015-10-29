<?php

/**
 * @file
 * Views handler: Affiliate2 commission structure field.
 */

/**
 * Return a value to display in the View.
 */
class uc_affiliate2_handler_field_commission_structure extends views_handler_field {
  function render($values) {
    return unserialize($values->uc_affiliate2_product_commission_commission_structure);
  }
}
