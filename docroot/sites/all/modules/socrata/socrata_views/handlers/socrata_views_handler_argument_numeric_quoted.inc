<?php
/**
 * @file
 *
 * Provides an argument handler for quoted numeric values
 */

class socrata_views_handler_argument_numeric_quoted extends socrata_views_handler_argument_numeric {
  function query($group_by = FALSE) {
    $this->query->add_where(0, $this->real_field, "'{$this->argument}'", '=');
  }
}
