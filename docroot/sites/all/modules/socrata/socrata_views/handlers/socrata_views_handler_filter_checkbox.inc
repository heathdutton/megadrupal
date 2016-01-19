<?php
/**
 * @file
 * Provides a filter handler to handle the "checkbox" field type in Socrata datasets.
 * Based heavily on Views core views_handler_filter_boolean_operator
 */

class socrata_views_handler_filter_checkbox extends views_handler_filter_boolean_operator {
  /**
   * Override parent method to provide the necessary SoQL predicates.
   */
  function query() {
    $field = $this->real_field;

    if (empty($this->value)) {
      $this->query->add_where_expression($this->options['group'], "({$field} is null or {$field} = false)");
    }
    else {
      $this->query->add_where($this->options['group'], $field, 'true', '=');
    }
  }
}
