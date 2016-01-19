<?php

/**
 * @file
 * Views handler: Node filter on "product-ness".
 */

/**
 * Filters product nodes .
 */
class basic_cart_handler_filter_product extends views_handler_filter_boolean_operator {

  /**
   * Overrides views_handler_field::query().
   */
  function query() {
    $types = basic_cart_product_types();
    $this->query->add_field('node', 'type');
    $this->query->add_where($this->options['group'], 'node.type', $types, empty($this->value) ? 'NOT IN' : 'IN');
  }
  
}