<?php
/**
 * @file
 * fancy_content_ctools_content.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function fancy_content_ctools_content_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_fancy_ctools_content'
  $field_bases['field_fancy_ctools_content'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_fancy_ctools_content',
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'ctools_field',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'ctools_field',
  );

  return $field_bases;
}
