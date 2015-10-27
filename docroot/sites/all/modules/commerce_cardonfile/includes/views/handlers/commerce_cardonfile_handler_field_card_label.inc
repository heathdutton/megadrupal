<?php

/**
 * Field handler to present the card label.
 */
class commerce_cardonfile_handler_field_card_label extends views_handler_field_entity {

  function get_value($values, $field = NULL) {
    if (!isset($this->entities[$this->view->row_index])) {
      return '';
    }

    $card = $this->entities[$this->view->row_index];
    return $card->label();
  }
}
