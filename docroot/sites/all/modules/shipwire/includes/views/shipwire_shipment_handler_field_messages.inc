<?php

/**
 * @file
 * Definition of shipwire_shipment_handler_field_messages.
 */

/**
 * Field handler to render shipment messages from array to list.
 *
 * @ingroup views_field_handlers
 */
class shipwire_shipment_handler_field_messages extends views_handler_field {

  function render($values) {
    $value = unserialize($this->get_value($values));
    return shipwire_shipment_render_messages($value);
  }

}
