<?php
/**
 * @file
 * timetable_feature_schedule.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function timetable_feature_schedule_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_schedule_timeslots'
  $field_bases['field_schedule_timeslots'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_schedule_timeslots',
    'foreign keys' => array(
      'eck_timeslot' => array(
        'columns' => array(
          'target_id' => 'id',
        ),
        'table' => 'eck_timeslot',
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
            'status' => 1,
          ),
          'session_room' => array(
            'status' => 0,
          ),
          'session_timeslot' => array(
            'status' => 0,
          ),
          'views-select-list' => array(
            'status' => 0,
          ),
        ),
        'sort' => array(
          'type' => 'none',
        ),
        'target_bundles' => array(
          'timeslot' => 'timeslot',
        ),
      ),
      'target_type' => 'timeslot',
    ),
    'translatable' => 0,
    'type' => 'entityreference',
  );

  // Exported field_base: 'field_session_timeslot'
  $field_bases['field_session_timeslot'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_session_timeslot',
    'foreign keys' => array(
      'eck_timeslot' => array(
        'columns' => array(
          'target_id' => 'id',
        ),
        'table' => 'eck_timeslot',
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
      'handler' => 'session_timeslot',
      'handler_settings' => array(
        'behaviors' => array(
          'schedule_timeslot' => array(
            'status' => 0,
          ),
          'session_room' => array(
            'status' => 0,
          ),
          'session_timeslot' => array(
            'status' => 1,
          ),
          'views-select-list' => array(
            'status' => 0,
          ),
        ),
        'target_bundles' => array(
          'timeslot' => 'timeslot',
        ),
      ),
      'target_type' => 'timeslot',
    ),
    'translatable' => 0,
    'type' => 'entityreference',
  );

  return $field_bases;
}
