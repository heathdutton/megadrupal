<?php
/**
 * @file
 * one_location.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function one_location_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-one_site_content-field_one_site_location'
  $field_instances['node-one_site_content-field_one_site_location'] = array(
    'bundle' => 'one_site_content',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'location_cck',
        'settings' => array(),
        'type' => 'location_default',
        'weight' => 6,
      ),
      'entityreference_view_widget' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'full' => array(
        'label' => 'hidden',
        'module' => 'location_cck',
        'settings' => array(),
        'type' => 'location_default',
        'weight' => 6,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'location_cck',
        'settings' => array(),
        'type' => 'location_default',
        'weight' => 4,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_one_site_location',
    'label' => 'Location',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'location_cck',
      'settings' => array(),
      'type' => 'location',
      'weight' => 8,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Location');

  return $field_instances;
}
