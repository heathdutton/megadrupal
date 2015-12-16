<?php
/**
 * @file
 * Provides an argument handler for text-based values
 */

class socrata_views_handler_argument_text extends socrata_views_handler_argument {
  /**
   * Set up the query for this argument.
   *
   * The argument sent may be found at $this->argument.
   */
  function query($group_by = FALSE) {
    $this->query->add_where(0, $this->real_field, "'{$this->argument}'", '=');
  }
}
