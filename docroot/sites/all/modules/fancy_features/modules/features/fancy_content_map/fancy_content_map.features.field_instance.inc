<?php
/**
 * @file
 * fancy_content_map.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function fancy_content_map_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'fancy_content-map-field_fancy_location'
  $field_instances['fancy_content-map-field_fancy_location'] = array(
    'bundle' => 'map',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'location_cck',
        'settings' => array(),
        'type' => 'location_map',
        'weight' => 0,
      ),
      'inline_entity_form_preview' => array(
        'label' => 'hidden',
        'module' => 'location_cck',
        'settings' => array(),
        'type' => 'location_default',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'fancy_content',
    'field_name' => 'field_fancy_location',
    'label' => 'Location',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'location_cck',
      'settings' => array(),
      'type' => 'location',
      'weight' => 3,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Location');

  return $field_instances;
}
