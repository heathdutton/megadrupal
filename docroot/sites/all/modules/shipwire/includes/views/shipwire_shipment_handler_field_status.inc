<?php

/**
 * @file
 * Definition of shipwire_shipment_handler_field_status.
 */

/**
 * Field handler to render translatable shipment status text.
 *
 * @ingroup views_field_handlers
 */
class shipwire_shipment_handler_field_status extends views_handler_field {

  function render($values) {
    $value = $this->get_value($values);
    $label = Shipwire::getStatusLabel($value);
    return $this->sanitize_value($label);
  }

}
