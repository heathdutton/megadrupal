<?php

/**
 * @file
 * Definition of shipwire_shipment_handler_field_shipwire_id.
 */

/**
 * Field handler to render the shipment shipwire id with optional link to the
 * shipwire merchant panel where the shipment can be edited.
 *
 * @ingroup views_field_handlers
 */
class shipwire_shipment_handler_field_shipwire_id extends views_handler_field_url {

  function construct() {
    parent::construct();
    $this->additional_fields['shipwire_uri'] = 'shipwire_uri';
    $this->additional_fields['shipwire_id'] = 'shipwire_id';
  }

  function render($values) {
    if (!empty($this->options['display_as_link'])) {
      $text = $this->get_value($values, 'shipwire_id');
      $uri = $this->get_value($values, 'shipwire_uri');
      if (!empty($this->options['text'])) {
        $text = $this->sanitize_value($this->options['text']);
      }
      if (!empty($text) && !empty($uri)) {
        $this->options['alter']['make_link'] = TRUE;
        $this->options['alter']['path'] = $uri;
      }
      return $text;
    }
    else {
      return $this->sanitize_value($values, 'shipwire_id');
    }
  }

}
