<?php
/**
 * @file
 * ideation.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function ideation_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_idea_categories'
  $field_bases['field_idea_categories'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_idea_categories',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        'Communication' => 'Communication',
        'Improvements' => 'Improvements',
        'Legislation' => 'Legislation',
        'Resources' => 'Resources',
        'Services' => 'Services',
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => 1,
    'type' => 'list_text',
  );

  // Exported field_base: 'field_idea_status'
  $field_bases['field_idea_status'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_idea_status',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        'Current Activity' => 'Current Activity',
        'In Action' => 'In Action',
        'No Status' => 'No Status',
        'Unable to Implement' => 'Unable to Implement',
        'Under Review' => 'Under Review',
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => 1,
    'type' => 'list_text',
  );

  // Exported field_base: 'field_idea_watch'
  $field_bases['field_idea_watch'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_idea_watch',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        0 => 'no',
        1 => 'yes',
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => 1,
    'type' => 'list_boolean',
  );

  return $field_bases;
}
