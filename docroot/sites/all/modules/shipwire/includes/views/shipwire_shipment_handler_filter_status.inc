<?php

/**
 * @file
 * Definition of shipwire_shipment_handler_filter_status.
 */

/**
 * Filter handler to restrict result by shipment status.
 *
 * @ingroup views_filter_handlers
 */
class shipwire_shipment_handler_filter_status extends views_handler_filter_in_operator {

  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_options = Shipwire::getStatusCodes();
    }
  }

}
