<?php
/**
 * @file
 * Views handler: Coupon filter on "product-type".
 */

/**
 * Filter coupons based on what product types they apply to
 */
class uc_coupon_handler_argument_product_type extends views_handler_argument {
  
  function query() {
    $this->ensure_my_table();
    $this->query->add_field('uc_coupons', 'data');
    if (!empty($this->argument)) {
      $regexp = '.*product_types.*{[^}]*(' . $this->argument . ').*}';
      $this->query->add_where_expression(0, 
      	'uc_coupons.data ' . 'REGEXP :re', 
        array(':re' => $regexp)
      );
    }
      //. ($this->options['operator'] !== 'in' ? "NOT " : "") 
  }

}
