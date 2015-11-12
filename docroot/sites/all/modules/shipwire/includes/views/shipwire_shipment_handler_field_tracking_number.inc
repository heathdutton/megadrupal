<?php

/**
 * @file
 * Definition of shipwire_shipment_handler_field_tracking_number.
 */

/**
 * Field handler to render a tracking number optionally linked to the remote 
 * tracking uri.
 *
 * @ingroup views_field_handlers
 */
class shipwire_shipment_handler_field_tracking_number extends views_handler_field_url {

  function construct() {
    parent::construct();
    $this->additional_fields['tracking_uri'] = 'tracking_uri';
    $this->additional_fields['tracking_number'] = 'tracking_number';
  }

  function render($values) {
    if (!empty($this->options['display_as_link'])) {
      $text = $this->get_value($values, 'tracking_number');
      if (!empty($this->options['text'])) {
        $text = $this->sanitize_value($this->options['text']);
      }
      if (!empty($text)) {
        $this->options['alter']['make_link'] = TRUE;
        $this->options['alter']['path'] = $this->get_value($values, 'tracking_uri');
      }
      return $text;
    }
    else {
      return $this->sanitize_value($values, 'tracking_number');
    }
  }

}
