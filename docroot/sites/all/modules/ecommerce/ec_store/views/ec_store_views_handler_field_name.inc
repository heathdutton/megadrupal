<?php

/**
 * @file
 * Views handler for displaying the customer name.
 */

/**
 * Field handler to provide simple renderer to display the customer name as
 * a single field
 */
class ec_store_views_handler_field_name extends views_handler_field {
  /**
   * Override the init function to add the additional fields for the names
   */
  function init(&$view, &$data) {
    parent::init($view, $data);
    $this->additional_fields['firstname'] = 'firstname';
    $this->additional_fields['lastname'] = 'lastname';
    $this->additional_fields['fullname'] = 'fullname';
  }

  function render($values) {
    $name = check_plain(!empty($values->{$this->aliases['fullname']}) ? $values->{$this->aliases['fullname']} : $values->{$this->aliases['firstname']} . ' ' . $values->{$this->aliases['lastname']});
    return $name;
  }
}
