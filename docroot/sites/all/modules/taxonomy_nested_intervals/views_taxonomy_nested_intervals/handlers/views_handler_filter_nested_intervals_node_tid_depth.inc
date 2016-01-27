<?php
/**
 * @file
 *
 * This file is a copy/override of the default views taxonomy filter handler.
 *
 * Filter handler for taxonomy terms with depth using Taxonomy Nested Intervals.
 *
 * @see views_handler_filter_term_node_tid_depth_modifier.inc
 */
class views_handler_filter_nested_intervals_node_tid_depth extends views_handler_filter_term_node_tid_depth {
  function query() {
    return parent::query();
  }
}
