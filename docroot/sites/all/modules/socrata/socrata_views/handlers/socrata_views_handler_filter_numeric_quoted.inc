<?php
/**
 * @file
 * Based heavily on views_handler_filter_numeric so we can take advantage of
 * its nice administrative UI and look like the usual Views numeric filter.
 */

class socrata_views_handler_filter_numeric_quoted extends socrata_views_handler_filter_numeric {
  /**
   * Override this method so we can wrap the value in single quotes
   * for SoSQL where needed.
   */
  function op_simple($field) {
    $this->query->add_where($this->options['group'], $field, "'{$this->value['value']}'", $this->operator);
  }
}
