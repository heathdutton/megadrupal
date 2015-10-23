<?php

/**
 * @file
 * Provides metadata for the line item entity.
 */

/**
 * Implements hook_entity_property_info().
 */
function studyroom_reservation_entity_property_info() {
  $info = array();

  // Add meta-data about the basic commerce_line_item properties.
  $properties = &$info['studyroom_reservation']['properties'];

  $properties['reservation_id'] = array(
    'label' => t('Reservation ID'),
    'description' => t('The internal numeric ID of the reservation.'),
    'type' => 'integer',
    'schema field' => 'reservation_id',
  );
  $properties['space_id'] = array(
    'label' => t('Space'),
    'type' => 'studyroom_space',
    'description' => t('The space of this reservation.'),
    'schema field' => 'space_id',
    'weight' => '-30',
  );
  $properties['uid'] = array(
    'label' => t('User'),
    'type' => 'user',
    'description' => t('The user who made this reservation.'),
    'schema field' => 'uid',
    'weight' => '1',
  );
  $properties['type'] = array(
    'label' => t('Reservation type'),
    'type' => 'text',
    'description' => t('The type of this reservation.'),
    'schema field' => 'type',
    'visible' => FALSE,
  );
  $properties['duration'] = array(
    'label' => t('Duration'),
    'description' => t('The duration of this reservation.'),
    'type' => 'integer',
    'getter callback' => 'studyroom_reservation_get_properties',
    'schema field' => 'duration',
    'weight' => '-1',
  );
  $properties['locked'] = array(
    'label' => t('Locked'),
    'description' => t('A boolean indicating whether the administrator may delete this reservation through the UI.'),
    'type' => 'integer',
    'schema field' => 'locked',
    'visible' => FALSE,
  );
  $properties['created'] = array(
    'label' => t('Date created'),
    'description' => t('The date the space was created.'),
    'type' => 'date',
    'schema field' => 'created',
    'visible' => FALSE,
  );
  $properties['changed'] = array(
    'label' => t('Date changed'),
    'description' => t('The date the space was most recently updated.'),
    'type' => 'date',
    'schema field' => 'changed',
    'visible' => FALSE,
  );

  return $info;
}
