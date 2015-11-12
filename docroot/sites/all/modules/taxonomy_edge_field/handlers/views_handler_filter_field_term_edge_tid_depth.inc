<?php
/**
 * @file
 * Filter handler for taxonomy terms with depth using Taxonomy Edge.
 *
 * @see views_handler_filter_term_node_tid_depth_modifier.inc
 */
class views_handler_filter_field_term_edge_tid_depth extends views_handler_filter_term_node_tid_depth {
  function query() {
    // If no filter values are present, then do nothing.
    if (count($this->value) == 0) {
      return;
    }
    elseif (is_array($this->value) && count($this->value) == 1) {
      $this->value = current($this->value);
    }

    $this->ensure_my_table();

    $tids = $this->value;
    $subquery = db_select('taxonomy_term_edge', 'te')
        ->fields('te', array('tid'));

    if ($this->options['depth'] == 'all') {
      $subquery->condition('te.parent', $tids);
    }
    elseif ($this->options['depth'] > 0) {
      $subquery
        ->condition('te.parent', $tids)
        ->condition('te.distance', (int) $this->options['depth'], '<=');
    }
    elseif ($this->options['depth'] < 0) {
      $subquery
        ->condition(
          'te.parent',
          db_select('taxonomy_term_edge', 'te2')
            ->fields('te2', array('parent'))
            ->condition('te2.tid', $tids)
            ->condition('distance', abs((int) $this->options['depth']))
        )
        ->condition('te.distance', abs((int) $this->options['depth']), '<=');
    }
    else {
      $subquery->condition('te.tid', $tids);
    }

    $this->query->add_where(0, "$this->table_alias.$this->real_field", $subquery, 'IN');
  }
}
