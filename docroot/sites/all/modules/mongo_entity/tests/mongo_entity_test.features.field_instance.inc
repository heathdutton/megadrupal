<?php
/**
 * @file
 * mongo_entity_test.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function mongo_entity_test_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'met_embedded_entity-met_embedded_entity-field_embedded_entity_name'
  $field_instances['met_embedded_entity-met_embedded_entity-field_embedded_entity_name'] = array(
    'bundle' => 'met_embedded_entity',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'met_embedded_entity',
    'field_name' => 'field_embedded_entity_name',
    'label' => 'Name',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 1,
    ),
  );

  // Exported field_instance: 'met_entity-met_entity-field_embedded_entity'
  $field_instances['met_entity-met_entity-field_embedded_entity'] = array(
    'bundle' => 'met_entity',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => NULL,
        'settings' => array(),
        'type' => 'embedded_entity_view',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'met_entity',
    'field_name' => 'field_embedded_entity',
    'label' => 'Embedded entity',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'mongo_entity',
      'settings' => array(),
      'type' => 'embedded_entity_widget',
      'weight' => 1,
    ),
  );

  // Exported field_instance: 'met_entity-met_entity-field_multi_embedded_entity'
  $field_instances['met_entity-met_entity-field_multi_embedded_entity'] = array(
    'bundle' => 'met_entity',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => NULL,
        'settings' => array(),
        'type' => 'embedded_entity_view',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'met_entity',
    'field_name' => 'field_multi_embedded_entity',
    'label' => 'Multi embedded entity',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'mongo_entity',
      'settings' => array(),
      'type' => 'embedded_entity_widget',
      'weight' => 2,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Embedded entity');
  t('Multi embedded entity');
  t('Name');

  return $field_instances;
}
