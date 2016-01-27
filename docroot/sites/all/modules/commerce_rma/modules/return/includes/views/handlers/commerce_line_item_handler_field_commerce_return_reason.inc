<?php

/**
 * Field handler to display the customer reason for a given returned product
 * stored in a commerce_line_item entity.
 */
class commerce_line_item_handler_field_commerce_return_reason extends views_handler_field {

  function construct() {
    parent::construct();
    $this->additional_fields['data'] = 'data';

    // Set real_field in order to make it generate a field_alias.
    $this->real_field = 'data';
  }

  function render($values) {
    $data = unserialize($this->get_value($values, 'data'));

    if (isset($data['commerce_return']['reason_tid'])) {
      $term = taxonomy_term_load($data['commerce_return']['reason_tid']);

      if ($term) {
        return t('@reason', array('@reason' => $term->name));
      }
    }

    return "";
  }
}
