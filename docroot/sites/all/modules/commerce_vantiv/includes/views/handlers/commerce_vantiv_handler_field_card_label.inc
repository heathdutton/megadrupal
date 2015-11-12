<?php

/**
 * Field handler to present the card label.
 */
class commerce_vantiv_handler_field_card_label extends views_handler_field_entity {

  function get_value($values, $field = NULL) {
    module_load_include('module', 'commerce_vantiv', 'commerce_vantiv');
    if (!isset($this->entities[$this->view->row_index])) {
      return '';
    }

    $card = $this->entities[$this->view->row_index];
    return commerce_vantiv_get_label($card);
  }
}
