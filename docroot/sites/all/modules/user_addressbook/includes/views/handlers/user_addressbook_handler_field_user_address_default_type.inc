<?php

/**
 * Field handler to translate a customer profile type into its readable form.
 */
class user_addressbook_handler_field_user_address_default_type extends views_handler_field {
  function render($values) {
    $type = $this->get_value($values);
    $types = user_addressbook_address_types();
    if (!empty($types[$type])) {
      $value = 'Default ' . $types[$type]['title'] . ' address';
    }
    else {
      $value = NULL;
    }

    return $this->sanitize_value($value);
  }
}
