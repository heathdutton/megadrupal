<?php

/**
 * Field handler to show the returned product status on commerce_line_item
 * level.
 */
class commerce_line_item_handler_field_commerce_return_status extends views_handler_field {

  function construct() {
    parent::construct();
    $this->additional_fields['data'] = 'data';

    // Set real_field in order to make it generate a field_alias.
    $this->real_field = 'data';
  }

  function render($values) {
    $data = unserialize($this->get_value($values, 'data'));

    if (isset($data['commerce_return']['line_item_status'])) {
      $statuses = commerce_return_line_item_statuses();

      if (isset($statuses[$data['commerce_return']['line_item_status']])) {
        return t('@status', array('@status' => $statuses[$data['commerce_return']['line_item_status']]['title']));
      }
    }

    return "";
  }
}
