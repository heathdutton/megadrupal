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
class views_handler_filter_nested_intervals_parent extends views_handler_filter_numeric {

  /**
   * Views filter handler query method.
   */
  public function query() {
    $this->ensure_my_table();

    // If we know which parents we are looking for, look the up directly.
    if (isset($this->value['value']) && is_numeric($this->value['value'])) {
      if ($this->value['value'] <= 0) {
        // Root parent, no lookup needed.
        return;
      }
      // Locate the parent(s) in the tree.
      $intervals = taxonomy_nested_intervals_get_intervals($this->value['value']);

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
    else {
      // We are unsure which parents we are looking for, let's self join.
      $join = new views_join();
      $join->construct('taxonomy_nested_intervals', $this->table_alias, 'vid', 'vid', array(), 'INNER');
      $alias = $this->query->add_table('taxonomy_nested_intervals', $this->relationship, $join);
      $this->query->add_where_expression(0, "$this->table_alias.lft > $alias.lft");
      $this->query->add_where_expression(0, "$this->table_alias.lft < $alias.rgt");
      $field = "$alias.tid";

      $info = $this->operators();
      if (!empty($info[$this->operator]['method'])) {
        $this->{$info[$this->operator]['method']}($field);
      }
    }
  }
}
