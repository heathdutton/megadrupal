<?php

/**
 * @file
 * This file is a copy/override of the default views taxonomy argument handler.
 *
 * Argument handler for taxonomy terms with depth using Taxonomy Edge.
 *
 * @see views_handler_argument_term_node_tid_depth.inc
 * @ingroup views_argument_handlers
 */
class views_handler_argument_field_term_edge_tid_depth extends views_handler_argument_term_node_tid_depth {

  function query($group_by = FALSE) {
    $this->ensure_my_table();

    if (!empty($this->options['break_phrase'])) {
      $tids = new stdClass();
      $tids->value = $this->argument;
      $tids = views_break_phrase($this->argument, $tids);
      if ($tids->value == array(-1)) {
        return FALSE;
      }
      $tids = $tids->value;
    }
    else {
      $tids = $this->argument;
    }
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
