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
class shipwire_shipment_handler_field_carrier_name extends views_handler_field {

  function construct() {
    parent::construct();
    $this->additional_fields['carrier_code'] = 'carrier_code';
  }

  function render($values) {
    $carrier_code = $this->get_value($values, 'carrier_code');
    return Shipwire::getCarrierName($carrier_code);
  }

}
