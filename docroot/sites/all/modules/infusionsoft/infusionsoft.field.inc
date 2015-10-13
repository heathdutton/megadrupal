<?php
/**
 * @file
 * File containing field definition for storing infusionsoft contact id.
 */

/**
 * Helper function to create an infusionsoft id field.
 */
function _infusionsoft_create_field() {
  // If a field type we know should exist isn't found, clear the Field cache.
  if (!field_info_field_types('number_integer')) {
    field_cache_clear();
  }

  $field_name = 'field_infusionsoft_id';

  // Look for or add the specified field to the requested entity bundle.
  $field = field_info_field($field_name);
  $instance = field_info_instance('user', $field_name, 'user');

  if (empty($field)) {
    $field = array(
      'field_name' => $field_name,
      'type' => 'number_integer',
      'cardinality' => 1,
      'entity_types' => array('user'),
      'translatable' => FALSE,
      'locked' => TRUE,
      'active' => '1',
      'deleted' => '0',
      'entity_types' => array('user'),
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(
        'profile2_private' => TRUE,
      ),
    );
    $field = field_create_field($field);
  }

  if (empty($instance)) {
    $instance = array(
      'field_name' => $field_name,
      'entity_type' => 'user',
      'bundle' => 'user',
      'label' => 'Infusionsoft ID',
      'settings' => array(
        'max' => '',
        'min' => '',
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => 0,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '13',
      ),
      'display' => array(
        'display' => array(
          'label' => 'hidden',
          'weight' => 1000,
        ),
      ),
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '3',
        ),
      ),
      'required' => 0,
    );
    field_create_instance($instance);
  }
}
