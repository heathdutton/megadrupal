<?php
/**
 * @file
 * Entity info hooks for commerce booking flexible pricing.
 */

/**
 * Implements hook_entity_property_info_alter().
 */
function commerce_booking_flexible_pricing_entity_property_info_alter(&$info) {
  $properties = &$info['commerce_order']['bundles']['commerce_booking']['properties'];

  $properties['booking_window_cut_off'] = array(
    'label' => t('Booking window cut off'),
    'description' => t('The cut off date of the current booking window.'),
    'type' => 'date',
    'getter callback' => 'commerce_booking_flexible_pricing_entity_property_get',
  );

  $properties['booking_window_increase'] = array(
    'label' => t('Booking window increase'),
    'description' => t('The price increase to the next booking window.'),
    'type' => 'text',
    'getter callback' => 'commerce_booking_flexible_pricing_entity_property_get',
  );
}
