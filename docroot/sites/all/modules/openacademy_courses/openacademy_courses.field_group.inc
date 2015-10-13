<?php
/**
 * @file
 * openacademy_courses.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function openacademy_courses_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_course_information|node|openacademy_course|form';
  $field_group->group_name = 'group_course_information';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'openacademy_course';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Course Information',
    'weight' => '2',
    'children' => array(
      0 => 'body',
      1 => 'field_course_people',
      2 => 'field_course_number',
      3 => 'field_course_credits',
      4 => 'field_course_type',
      5 => 'field_course_term',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Course Information',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_course_information|node|openacademy_course|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_course_scheduling|node|openacademy_course|form';
  $field_group->group_name = 'group_course_scheduling';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'openacademy_course';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Course Scheduling',
    'weight' => '6',
    'children' => array(
      0 => 'field_course_location',
      1 => 'field_course_time',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Course Scheduling',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_course_scheduling|node|openacademy_course|form'] = $field_group;

  return $export;
}
