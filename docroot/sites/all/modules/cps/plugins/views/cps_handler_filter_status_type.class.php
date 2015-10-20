<?php

/**
 * @file
 * Definition of cps_handler_filter_status_type.
 */

/**
 * Filter by node type.
 *
 * @ingroup views_filter_handlers
 */
class cps_handler_filter_status_type extends views_handler_filter_in_operator {
  /**
   * @{inheritdoc}
   */
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_options = cps_changeset_get_state_types();
    }
  }

  /**
   * @{inheritdoc}
   */
  function op_simple() {
    if (empty($this->value)) {
      return;
    }
    $this->ensure_my_table();

    $types = array_flip(array_values($this->value));
    $states = cps_changeset_get_states();

    $values = array();
    foreach ($states as $id => $state) {
      if (isset($state['type']) && isset($types[$state['type']])) {
        $values[] = $id;
      }
    }

    if (!$values) {
      return;
    }

    $this->query->add_where($this->options['group'], "$this->table_alias.$this->real_field", $values, $this->operator);
  }

}
