<?php

/**
 * @file
 * Provides info about the space entity.
 */

/**
 * Implements hook_entity_property_info().
 */
function studyroom_space_entity_property_info() {
  $info = array();
  $properties = &$info['studyroom_space']['properties'];

  $properties['space_id'] = array(
    'label' => t('Space ID'),
    'description' => t('The internal numeric ID of the space.'),
    'type' => 'integer',
    'schema field' => 'space_id',
  );
  $properties['type'] = array(
    'label' => t('Type'),
    'description' => t('The location of this space.'),
    'type' => 'token',
    'options list' => 'entity_metadata_field_options_list',
    'required' => TRUE,
    'schema field' => 'type',
    'visible' => FALSE,
  );
  $properties['language'] = array(
    'label' => t('Language'),
    'type' => 'token',
    'description' => t('The language the space was created in.'),
    'options list' => 'entity_metadata_language_list',
    'schema field' => 'language',
    'visible' => FALSE,
  );
  $properties['location'] = array(
    'label' => t("Location"),
    'description' => t("The location for this space."),
    'type' => 'text',
    'getter callback' => 'studyroom_space_get_property',
    'computed' => TRUE,
  );
  $properties['label'] = array(
    'label' => t('Label'),
    'description' => t('The label displayed with this space.'),
    'type' => 'text',
    'schema field' => 'label',
    'weight' => '-30',
  );
  $properties['capacity'] = array(
    'label' => t('Capacity'),
    'description' => t('The capacity of this space.'),
    'type' => 'integer',
    'required' => TRUE,
    'schema field' => 'capacity',
    'weight' => '0',
  );
  $properties['min_duration'] = array(
    'label' => t('Minimum duration'),
    'description' => t('The minimum length of time (in seconds) of a reservation for this space.'),
    'type' => 'integer',
    'getter callback' => 'studyroom_space_get_property',
    'schema field' => 'min_duration',
    'visible' => FALSE,
  );
  $properties['max_duration'] = array(
    'label' => t('Maximum duration'),
    'description' => t('The maximum length of time (in seconds) of a reservation for this space.'),
    'type' => 'integer',
    'getter callback' => 'studyroom_space_get_property',
    'schema field' => 'max_duration',
    'visible' => FALSE,
  );
  $properties['max_days_advance'] = array(
    'label' => t('Maximum days advance'),
    'description' => t('The maximum number of days in advance to allow a reservation for this space.'),
    'type' => 'integer',
    'getter callback' => 'studyroom_space_get_property',
    'schema field' => 'max_days_advance',
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
  $properties['status'] = array(
    'label' => t("Status"),
    'description' => t("Whether the space is published or unpublished."),
    'type' => 'integer',
    'options list' => 'entity_metadata_status_options_list',
    'schema field' => 'status',
    'visible' => FALSE,
  );

  return $info;
}
