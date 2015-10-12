<?php

/**
 * @file
 * Definition of views_handler_filter_workflow_state.
 */

/**
 * Filter by node type.
 *
 * @ingroup views_filter_handlers
 */
class views_handler_filter_gcc_current_context extends views_handler_filter {

  var $no_operator = TRUE;

  function can_expose() { return FALSE; }

  function query() {

    $this->ensure_my_table();

    $context = gcc_context_get_context();

    if (!$context) {
      $this->query->add_where($this->options['group'], $this->table_alias . '.entity_type', 'none');
      $this->query->add_where($this->options['group'], $this->table_alias . '.entity_id', 0);
    }
    else {
      $this->query->add_where($this->options['group'], $this->table_alias . '.entity_type', $context['entity_type']);
      $this->query->add_where($this->options['group'], $this->table_alias . '.entity_id', $context['entity_id']);
    }
  }
}
