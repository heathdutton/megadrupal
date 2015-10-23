<?php
/**
 * @file
 * app_example.features.inc
 */

/**
 * Implements hook_views_api().
 */
function app_example_views_api() {
  return array("api" => "3.0");
}

/**
 * Implements hook_eck_bundle_info().
 */
function app_example_eck_bundle_info() {
  $items = array(
  'eck_employee_employee' => array(
  'machine_name' => 'eck_employee_employee',
  'entity_type' => 'eck_employee',
  'name' => 'employee',
  'label' => 'Employee',
),
  );
  return $items;
}

/**
 * Implements hook_eck_entity_type_info().
 */
function app_example_eck_entity_type_info() {
$items = array(
       'eck_employee' => array(
  'name' => 'eck_employee',
  'label' => 'Employee',
  'properties' => array(
    'name' => array(
      'label' => 'Name',
      'type' => 'text',
      'behavior' => NULL,
    ),
  ),
),
  );
  return $items;
}

/**
 * Implements hook_node_info().
 */
function app_example_node_info() {
  $items = array(
    'meeting_room' => array(
      'name' => t('Meeting room'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implements hook_default_og_membership_type().
 */
function app_example_default_og_membership_type() {
  $items = array();
  $items['room_booking'] = entity_import('og_membership_type', '{
    "name" : "room_booking",
    "description" : "Room booking",
    "language" : "",
    "rdf_mapping" : []
  }');
  return $items;
}
