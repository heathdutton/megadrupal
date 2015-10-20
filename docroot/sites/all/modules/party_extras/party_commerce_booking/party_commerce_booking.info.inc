<?php

/**
 * Implements hook_entity_property_info_alter().
 *
 * Add booking references for search api to index.
 */
function party_commerce_booking_entity_property_info_alter(&$property_info) {
  $properties =& $property_info['party']['properties'];

  $properties['booking_reference_manager'] = array(
    'label' => t('Booking References (Booking Manager)'),
    'description' => t('The booking references that this party manages.'),
    'type' => 'list<text>',
    'computed' => TRUE,
    'getter callback' => 'party_commerce_booking_property_booking_reference_manager_get',
  );

  $properties['booking_reference_tickets'] = array(
    'label' => t('Booking References (Tickets)'),
    'description' => t('The booking references that this party has tickets for.'),
    'type' => 'list<text>',
    'computed' => TRUE,
    'getter callback' => 'party_commerce_booking_property_booking_reference_tickets_get',
  );

  $properties['event_tickets'] = array(
    'label' => t('Event tickets'),
    'description' => t('The events that this party has tickets for.'),
    'type' => 'list<node>',
    'computed' => TRUE,
    'getter callback' => 'party_commerce_booking_property_event_tickets_get',
  );

}
