<?php
/**
 * @file
 * nm_events.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function nm_events_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_nm_event_description|node|nm_event|form';
  $field_group->group_name = 'group_nm_event_description';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'nm_event';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_nm_event_information';
  $field_group->data = array(
    'label' => 'Event Description',
    'weight' => '37',
    'children' => array(
      0 => 'field_nm_body',
      1 => 'field_nm_event_link',
      2 => 'field_nm_registration_info',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_nm_event_description|node|nm_event|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_nm_event_details|node|nm_event|form';
  $field_group->group_name = 'group_nm_event_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'nm_event';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_nm_event_information';
  $field_group->data = array(
    'label' => 'Event Details',
    'weight' => '36',
    'children' => array(
      0 => 'field_nm_address',
      1 => 'field_nm_date',
      2 => 'title',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_nm_event_details|node|nm_event|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_nm_event_information|node|nm_event|form';
  $field_group->group_name = 'group_nm_event_information';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'nm_event';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Event Information',
    'weight' => '0',
    'children' => array(
      0 => 'group_nm_event_description',
      1 => 'group_nm_event_details',
      2 => 'group_nm_event_media',
      3 => 'group_nm_event_relation',
    ),
    'format_type' => 'tabs',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );
  $export['group_nm_event_information|node|nm_event|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_nm_event_media|node|nm_event|form';
  $field_group->group_name = 'group_nm_event_media';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'nm_event';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_nm_event_information';
  $field_group->data = array(
    'label' => 'Media Assets',
    'weight' => '38',
    'children' => array(
      0 => 'field_nm_headline_image',
      1 => 'field_nm_attach_gallery',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_nm_event_media|node|nm_event|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_nm_event_relation|node|nm_event|form';
  $field_group->group_name = 'group_nm_event_relation';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'nm_event';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_nm_event_information';
  $field_group->data = array(
    'label' => 'Terms & Relation',
    'weight' => '39',
    'children' => array(
      0 => 'field_nm_tags',
      1 => 'field_nm_related_content',
      2 => 'field_nm_event_category',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_nm_event_relation|node|nm_event|form'] = $field_group;

  return $export;
}
