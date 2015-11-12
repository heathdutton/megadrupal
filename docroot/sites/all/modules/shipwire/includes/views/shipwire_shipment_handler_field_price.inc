<?php

/**
 * @file
 * Definition of shipwire_shipment_handler_field_price.
 */

/**
 * Field handler to format a price with currency.
 *
 * @ingroup views_field_handlers
 */
class shipwire_shipment_handler_field_price extends views_handler_field {

  function construct() {
    parent::construct();
    $this->additional_fields['currency'] = 'currency';
  }

  function render($values) {
    $currency_code = $this->get_value($values, 'currency');
    $amount = $this->get_value($values);
    return shipwire_currency_format($amount, $currency_code);
  }

}
