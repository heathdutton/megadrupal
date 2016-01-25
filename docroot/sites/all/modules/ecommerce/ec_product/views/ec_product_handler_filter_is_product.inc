<?php

/**
 * @file
 * Provide filter for product nodes.
 */

class ec_product_handler_filter_is_product extends views_handler_filter_boolean_operator {
  function query() {
    $table = $this->ensure_my_table();
    if ($this->value) {
      $this->query->add_where_expression($this->options['group'], "$table.ptype IS NOT NULL");
    }
    else {
      $this->query->add_where_expression($this->options['group'], "$table.ptype IS NULL");
    }
  }
}
