<?php

/**
 * @file
 * Views handler for field Domain Type.
 */

class views_handler_field_domain_type extends views_handler_field {

  function render($values) {
    $value = $this->get_value($values);

    $types = domain_types();

    return $types[$value]['name'];
  }

}
