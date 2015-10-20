<?php
/**
 * @file
 * timetable_feature_timeslot.features.inc
 */

/**
 * Implements hook_eck_bundle_info().
 */
function timetable_feature_timeslot_eck_bundle_info() {
  $items = array(
  'timeslot_timeslot' => array(
  'machine_name' => 'timeslot_timeslot',
  'entity_type' => 'timeslot',
  'name' => 'timeslot',
  'label' => 'Timeslot',
),
  );
  return $items;
}

/**
 * Implements hook_eck_entity_type_info().
 */
function timetable_feature_timeslot_eck_entity_type_info() {
$items = array(
       'timeslot' => array(
  'name' => 'timeslot',
  'label' => 'Timeslot',
  'properties' => array(),
),
  );
  return $items;
}
