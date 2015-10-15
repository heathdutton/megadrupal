<?php
/**
 * @file
 * timetable_feature_rooms.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function timetable_feature_rooms_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_session_room'
  $field_bases['field_session_room'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_session_room',
    'foreign keys' => array(
      'eck_room' => array(
        'columns' => array(
          'target_id' => 'id',
        ),
        'table' => 'eck_room',
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
          'schedule_timeslot' => array(
            'status' => 0,
          ),
          'session_room' => array(
            'status' => 1,
          ),
          'session_timeslot' => array(
            'status' => 0,
          ),
          'views-select-list' => array(
            'status' => 0,
          ),
        ),
        'sort' => array(
          'direction' => 'ASC',
          'property' => 'title',
          'type' => 'property',
        ),
        'target_bundles' => array(),
      ),
      'target_type' => 'room',
    ),
    'translatable' => 0,
    'type' => 'entityreference',
  );

  return $field_bases;
}
