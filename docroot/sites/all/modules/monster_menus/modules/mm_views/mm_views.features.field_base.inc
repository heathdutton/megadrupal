<?php
/**
 * @file
 * mm_views.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function mm_views_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_node_view'
  $field_bases['field_node_view'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_node_view',
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'viewfield',
    'settings' => array(
      'super_default' => 0,
      'super_default_widget' => array(
        'vargs' => '',
        'vname' => '',
      ),
      'token_enabled' => '',
    ),
    'translatable' => 0,
    'type' => 'viewfield',
    'type_name' => 'view_node',
  );

  return $field_bases;
}
