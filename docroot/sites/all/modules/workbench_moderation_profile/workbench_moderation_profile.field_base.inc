<?php
/**
 * @file
 * workbench_moderation_profile.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function workbench_moderation_profile_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_transitions'
  $field_bases['field_transitions'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_transitions',
    'foreign keys' => array(
      'workbench_moderation_transitions' => array(
        'columns' => array(
          'target_id' => 'id',
        ),
        'table' => 'workbench_moderation_transitions',
      ),
    ),
    'indexes' => array(
      'target_id' => array(
        0 => 'target_id',
      ),
    ),
    'locked' => 0,
    'module' => 'entityreference',
    'settings' => array(
      'handler' => 'base',
      'handler_settings' => array(
        'behaviors' => array(
          'views-select-list' => array(
            'status' => 0,
          ),
        ),
        'sort' => array(
          'type' => 'none',
        ),
        'target_bundles' => array(),
      ),
      'target_type' => 'workbench_moderation_transition',
    ),
    'translatable' => 0,
    'type' => 'entityreference',
  );

  return $field_bases;
}
