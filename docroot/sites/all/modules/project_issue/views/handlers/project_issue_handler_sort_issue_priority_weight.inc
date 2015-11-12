<?php

/**
 * Sort handler for issue priority, using denormalized priority_weight column.
 */
class project_issue_handler_sort_issue_priority_weight extends views_handler_sort {
  function query() {
    $this->ensure_my_table();
    // Reverse the sort order since more important priorities are weighted
    // lighter.
    $order = ($this->options['order'] == 'asc') ? 'desc' : 'asc';
    // Override the field name, since we are storing a denormalized weight.
    $this->query->add_orderby($this->table_alias, 'priority_weight', $order);
  }
}
