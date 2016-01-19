<?php
/**
 * @file
 */

/**
 * Views sort handler to sort by taxonomy terms' hierarchy order.
 */
class views_handler_sort_nested_intervals_hierarchy extends views_handler_sort {
  /**
   * Overridden to add the ORDER BY clause and join required tables.
   */
  function query() {
    $alias = $this->query->ensure_table('taxonomy_nested_intervals');
    $this->query->add_orderby(NULL, NULL, $this->options['order'], "$alias.lft");
  }
}
