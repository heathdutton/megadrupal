<?php
/**
 * @file
 * openacademy_events.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function openacademy_events_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_event_details|node|openacademy_event|form';
  $field_group->group_name = 'group_event_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'openacademy_event';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Event Details',
    'weight' => '3',
    'children' => array(
      0 => 'field_event_location',
      1 => 'field_event_moreinfo',
      2 => 'field_event_type',
      3 => 'field_tags',
      4 => 'field_event_address',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Event Details',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_event_details|node|openacademy_event|form'] = $field_group;

  return $export;
}
