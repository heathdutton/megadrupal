<?php
/**
 * @file
 * timetable_feature_rooms.features.inc
 */

/**
 * Implements hook_views_api().
 */
function timetable_feature_rooms_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_eck_bundle_info().
 */
function timetable_feature_rooms_eck_bundle_info() {
  $items = array(
  'room_room' => array(
  'machine_name' => 'room_room',
  'entity_type' => 'room',
  'name' => 'room',
  'label' => 'Room',
),
  );
  return $items;
}

/**
 * Implements hook_eck_entity_type_info().
 */
function timetable_feature_rooms_eck_entity_type_info() {
$items = array(
       'room' => array(
  'name' => 'room',
  'label' => 'Room',
  'properties' => array(
    'title' => array(
      'label' => 'Title',
      'type' => 'text',
      'behavior' => 'title',
    ),
  ),
),
  );
  return $items;
}
