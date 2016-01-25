<?php
/**
 * @file
 */
class views_join_nested_intervals extends views_join {
  function build_join($select_query, $table, $view_query) {
    $this->extra = $table['alias'] . '.vid = ' . $this->left_table . '.vid';
    parent::build_join($select_query, $table, $view_query);
  }
}
