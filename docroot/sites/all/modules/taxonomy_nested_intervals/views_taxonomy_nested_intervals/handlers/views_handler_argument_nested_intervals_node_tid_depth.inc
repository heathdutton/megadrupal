<?php
/**
 * @file
 * This file is a copy/override of the default views taxonomy argument handler.
 *
 * Argument handler for taxonomy terms with depth using Taxonomy Nested Intervals.
 *
 * @see views_handler_argument_term_node_tid_depth.inc
 * @ingroup views_argument_handlers
 */

class views_handler_argument_nested_intervals_node_tid_depth extends views_handler_argument_term_node_tid_depth {
  function query($group_by = FALSE) {
    if ($this->options['depth'] == 'all') {
      $this->options['depth'] = taxonomy_nested_intervals_get_max_depth();
    }
    elseif ($this->options['depth'] == 'root') {
      $this->options['depth'] = -taxonomy_nested_intervals_get_max_depth();
    }
    return parent::query($group_by);
  }
}
