<?php

/**
 * @file
 * Definition of views_handler_filter_numeric.
 */

/**
 * Simple filter to handle greater than/less than filters
 *
 * @ingroup views_filter_handlers
 */
class views_handler_argument_nested_intervals_parent extends views_handler_argument_numeric {

  /**
   * Views argument handler query method.
   */
  public function query() {
    $this->ensure_my_table();

    if (!empty($this->options['break_phrase'])) {
      views_break_phrase($this->argument, $this);
    }
    else {
      $this->value = array($this->argument);
    }

    // If we know which parents we are looking for, look the up directly.
    foreach ($this->value as $value) {
      if ($value <= 0) {
        // Root parent, no lookup needed.
        continue;
      }
      // Locate the parent(s) in the tree.
      $intervals = taxonomy_nested_intervals_get_intervals($value);

      // Use those parents as scope.
      foreach ($intervals as $interval) {
        $this->query->add_where(
          'nested_intervals_parents',
          db_and()
            ->where("$this->table_alias.lft > $interval->lft")
            ->where("$this->table_alias.lft < $interval->rgt")
        );
      }
      $this->query->set_where_group('OR', 'nested_intervals_parents');
    }
  }
}
