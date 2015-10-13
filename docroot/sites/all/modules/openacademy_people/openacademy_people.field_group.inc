<?php
/**
 * @file
 * openacademy_people.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function openacademy_people_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_person_contact|node|openacademy_person|form';
  $field_group->group_name = 'group_person_contact';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'openacademy_person';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Contact Information',
    'weight' => '3',
    'children' => array(
      0 => 'field_person_email',
      1 => 'field_person_office',
      2 => 'field_person_officehours',
      3 => 'field_person_phone',
      4 => 'field_person_website',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Contact Information',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_person_contact|node|openacademy_person|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_person_information|node|openacademy_person|form';
  $field_group->group_name = 'group_person_information';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'openacademy_person';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Person Information',
    'weight' => '2',
    'children' => array(
      0 => 'body',
      1 => 'field_person_courses',
      2 => 'field_person_position',
      3 => 'field_person_publications',
      4 => 'field_person_type',
      5 => 'field_tags',
      6 => 'field_person_cv',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Person Information',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_person_information|node|openacademy_person|form'] = $field_group;

  return $export;
}
