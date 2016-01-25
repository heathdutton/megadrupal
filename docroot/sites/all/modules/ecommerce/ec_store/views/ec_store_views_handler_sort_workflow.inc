<?php

/**
 * @file
 * Provide handler to allow the sorting on the workflow in transactions.
 */

class ec_store_views_handler_sort_workflow extends views_handler_sort {
  function query() {
    $this->ensure_my_table();
    // Add the field.
    $this->query->add_orderby($this->table_alias, 'weight', $this->options['order']);
  }
}

