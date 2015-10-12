<?php
/**
 * @file
 * cm_agenda_live.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function cm_agenda_live_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_cm_agenda_delay_time'
  $field_bases['field_cm_agenda_delay_time'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_cm_agenda_delay_time',
    'indexes' => array(),
    'locked' => 0,
    'module' => 'hms_field',
    'settings' => array(
      'profile2_private' => FALSE,
    ),
    'translatable' => 0,
    'type' => 'hms_field',
  );

  // Exported field_base: 'field_cm_agenda_live'
  $field_bases['field_cm_agenda_live'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_cm_agenda_live',
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
      'profile2_private' => FALSE,
      'uri_scheme' => 'public',
    ),
    'translatable' => 0,
    'type' => 'file',
  );

  // Exported field_base: 'field_cm_agenda_start_viewer'
  $field_bases['field_cm_agenda_start_viewer'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_cm_agenda_start_viewer',
    'indexes' => array(),
    'locked' => 0,
    'module' => 'hms_field',
    'settings' => array(
      'profile2_private' => FALSE,
    ),
    'translatable' => 0,
    'type' => 'hms_field',
  );

  // Exported field_base: 'field_cm_agenda_workflow_state'
  $field_bases['field_cm_agenda_workflow_state'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_cm_agenda_workflow_state',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        'restrict' => 'No access for anonymous users',
        'live' => 'Show live video (or countdown if before start time)',
        'post_live' => 'Show message after broadcast',
        'vod' => 'Show Video on demand',
      ),
      'allowed_values_function' => '',
      'profile2_private' => FALSE,
    ),
    'translatable' => 0,
    'type' => 'list_text',
  );

  return $field_bases;
}
