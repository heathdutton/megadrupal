<?php

class views_arguments_handler_relationship_child_terms extends views_handler_relationship {

  /**
   * Called to implement a relationship in a query.
   */
  function query() {
    $this->ensure_my_table();

    // LEFT JOIN taxonomy_term_hierarchy ch ON THIS.tid = ch.parent
    $join = new views_join;
    $join->table = 'taxonomy_term_hierarchy';
    $join->field = 'parent';
    $join->left_table = $this->table_alias;
    $join->left_field = 'tid';
    $join->type = 'left';
    $alias = $this->query->add_relationship('ch', $join, 'taxonomy_term_hierarchy');

    // LEFT JOIN taxonomy_term_data ct ON ch.tid = ct.tid
    $join = new views_join;
    $join->table = 'taxonomy_term_data';
    $join->field = 'tid';
    $join->left_table = $alias;
    $join->left_field = 'tid';
    $join->type = 'left';
    $this->alias = $this->query->add_relationship('ct', $join, 'taxonomy_term_data');
  }

}
