<?php
/**
 * @file
 * timetable_feature_timeslot.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function timetable_feature_timeslot_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_timeslot_time'
  $field_bases['field_timeslot_time'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_timeslot_time',
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'timefield',
    'settings' => array(
      'totime' => 'required',
      'weekly_summary' => 0,
      'weekly_summary_with_label' => 0,
    ),
    'translatable' => 0,
    'type' => 'timefield',
  );

  return $field_bases;
}
