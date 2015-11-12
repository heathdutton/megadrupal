<?php
/**
 * @file
 * Rules integration for commerce_booking.
 */

/**
 * Implements hook_rules_condition_info().
 */
function commerce_booking_rules_condition_info() {
  return array(
    'commerce_booking_condition_entity_booking_open' => array(
      'label' => t('Entity is open for booking'),
      'parameter' => array(
        'entity' => array(
          'label' => t('Entity'),
          'type' => 'entity',
          'wrapped' => TRUE,
        ),
      ),
      'group' => t('Commerce Booking'),
    ),
  );
}

/**
 * Rules condition for checking whether an entity is open for booking.
 *
 * @param EntityMetadataWrapper $entity_wrapper
 *   An entity metadata wrapper of the entity we're checking.
 */
function commerce_booking_condition_entity_booking_open($entity_wrapper) {
  $booking_field = commerce_booking_get_field_name($entity_wrapper->type(), $entity_wrapper->value());
  return isset($entity_wrapper->{$booking_field}) && $entity_wrapper->{$booking_field}->booking_open->value() == COMMERCE_BOOKING_BOOKING_OPEN;
}
