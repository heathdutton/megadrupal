<?php
/**
 * @file
 * panopoly_seo.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function panopoly_seo_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_google_url'
  $field_bases['field_google_url'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_google_url',
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
