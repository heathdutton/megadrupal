<?php
/**
 * @file
 * Filter by whether a node is indexed or not.
 */
class node_noindex_filter_handler extends views_handler_filter_boolean_operator {

  function construct() {
    parent::construct();
    $this->value_value = t('Is indexed');
  }

  function query() {
    $this->ensure_my_table();
    if ($this->value == 1) {
      $this->query->add_where_expression(
        $this->options['group'],
        "$this->table_alias.noindex is NULL OR $this->table_alias.noindex = 0",
        array()
      );
    }
    else {
      $this->query->add_where($this->options['group'], "$this->table_alias.noindex", "1", "=");
    }
  }
}

