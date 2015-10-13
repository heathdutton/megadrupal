<?php

/**
 * @file
 * Views handler: Link to add node to compare..
 */

/**
 * Field handler to display “Add to Compare” link for nodes.
 */
class node_compare_handler_field_compare_link extends views_handler_field {
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function element_type($none_supported = FALSE, $default_empty = FALSE, $inline = FALSE) {
    if (isset($this->definition['element type'])) {
      return $this->definition['element type'];
    }
    return 'div';
  }

  function render($values) {
    $type = $values->{$this->aliases['type']};
    if (variable_get('node_compare_type_' . $type, array())) {
      return theme('node_compare_toggle_link', array('nid' => $values->{$this->aliases['nid']}));
    }
  }
}
