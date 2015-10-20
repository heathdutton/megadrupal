<?php

/**
 * @file
 * Definition of shipwire_shipment_handler_field_link_edit.
 */

/**
 * Field handler to present a link to edit a shipment.
 *
 * @ingroup views_field_handlers
 */
class shipwire_shipment_handler_field_link_edit extends shipwire_shipment_handler_field_link_view {

  function construct() {
    parent::construct();
    $this->additional_fields['shipwire_shipment_id'] = 'shipwire_shipment_id';
  }

  function render($values) {
    $shipment_id = $this->get_value($values, 'shipwire_shipment_id');
    $text = !empty($this->options['text']) ? $this->options['text'] : t('Edit');
    return l($text, 'admin/shipwire/shipment/' . $shipment_id . '/edit', array('query' => drupal_get_destination()));
  }

}
