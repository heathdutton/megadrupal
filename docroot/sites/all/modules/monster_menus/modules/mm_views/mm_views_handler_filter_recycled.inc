<?php

/**
 * @file
 * Views interface for Monster Menus
 */

class mm_views_handler_filter_recycled extends views_handler_filter_boolean_operator {
  function query() {
    $this->ensure_my_table();
    $this->query->add_where($this->options['group'], "$this->table_alias.id", NULL, empty($this->value) ? 'IS NULL' : 'IS NOT NULL');
  }
}
