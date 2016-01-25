<?php
/**
 * @file
 * mongo_entity_test.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function mongo_entity_test_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_embedded_entity'
  $field_bases['field_embedded_entity'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_embedded_entity',
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'mongo_entity',
    'settings' => array(
      'entity_type' => 'met_embedded_entity',
    ),
    'translatable' => 0,
    'type' => 'embedded_entity',
  );

  // Exported field_base: 'field_embedded_entity_name'
  $field_bases['field_embedded_entity_name'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_embedded_entity_name',
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'max_length' => 255,
    ),
    'translatable' => 0,
    'type' => 'text',
  );

  // Exported field_base: 'field_multi_embedded_entity'
  $field_bases['field_multi_embedded_entity'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_multi_embedded_entity',
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'mongo_entity',
    'settings' => array(
      'entity_type' => 'met_embedded_entity',
    ),
    'translatable' => 0,
    'type' => 'embedded_entity',
  );

  return $field_bases;
}
