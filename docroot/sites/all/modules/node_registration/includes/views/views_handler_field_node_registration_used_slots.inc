<?php

/**
 * @file
 * 
 */

class views_handler_field_node_registration_used_slots extends views_handler_field_numeric {

  function query() {
    $this->query->add_field(NULL, '(SELECT SUM(slots) FROM {node_registration} WHERE nid = node.nid AND cancelled = 0)', 'registration_used_slots');
  }

  function render($values) {
    $values = (array) $values;
    return (int) $values['registration_used_slots'];
  }

}
