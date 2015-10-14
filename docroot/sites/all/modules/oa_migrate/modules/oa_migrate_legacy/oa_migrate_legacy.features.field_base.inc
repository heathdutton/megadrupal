<?php
/**
 * @file
 * oa_migrate_legacy.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function oa_migrate_legacy_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_oa_migrate_legacy_id'
  $field_bases['field_oa_migrate_legacy_id'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_oa_migrate_legacy_id',
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

  return $field_bases;
}
