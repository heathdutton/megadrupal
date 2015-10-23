<?php
/**
 * @file
 * fancy_section_responsive.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function fancy_section_responsive_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_fancy_view_mode_selector'
  $field_bases['field_fancy_view_mode_selector'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_fancy_view_mode_selector',
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'view_mode_selector',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'view_mode_selector',
  );

  return $field_bases;
}
