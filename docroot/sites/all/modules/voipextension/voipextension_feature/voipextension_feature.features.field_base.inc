<?php
/**
 * @file
 * voipextension_feature.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function voipextension_feature_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_audio'
  $field_bases['field_voipextension_audio'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_voipextension_audio',
    'foreign keys' => array(
      'fid' => array(
        'columns' => array(
          'fid' => 'fid',
        ),
        'table' => 'file_managed',
      ),
    ),
    'indexes' => array(
      'fid' => array(
        0 => 'fid',
      ),
    ),
    'locked' => 0,
    'module' => 'file',
    'settings' => array(
      'display_default' => 0,
      'display_field' => 0,
      'uri_scheme' => 'public',
    ),
    'translatable' => 0,
    'type' => 'file',
  );

  // Exported field_base: 'field_extension'
  $field_bases['field_voipextension'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_voipextension',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'voipextension',
    'settings' => array(
      'auto' => 1,
      'max_length' => 4,
    ),
    'translatable' => 0,
    'type' => 'voipextension',
  );

  return $field_bases;
}
