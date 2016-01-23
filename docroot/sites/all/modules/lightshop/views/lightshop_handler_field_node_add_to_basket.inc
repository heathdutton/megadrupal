<?php

/**
 * @file
 * Views handler: add-to-basket button.
 */

/**
 * Field handler to display basket button for nodes certain type of.
 */
class lightshop_handler_field_node_add_to_basket extends views_handler_field {
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
    $basket_types = variable_get('lightshop_node_types', array());
    if (in_array($values->{$this->aliases['type']}, $basket_types)) {
      return lightshop_add2basket_link($values->{$this->aliases['nid']});
    }
  }
}

