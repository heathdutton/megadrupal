<?php
/**
 * @file
 * xcri_cap.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function xcri_cap_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_inherited|node|course_presentation|form';
  $field_group->group_name = 'group_inherited';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'course_presentation';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Override inherited values',
    'weight' => '19',
    'children' => array(
      0 => 'field_subject',
      1 => 'field_regulations',
      2 => 'field_prerequisite',
      3 => 'field_objective',
      4 => 'field_learning_outcome',
      5 => 'field_assessment',
      6 => 'field_application_procedure',
      7 => 'body',
      8 => 'field_abstract',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Override inherited values',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => ' group-inherited field-group-fieldset',
        'description' => 'You can override course\'s taxonomy values for this course presentation. if any of the following fields are empty, the course\'s taxonomy values will be used.',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_inherited|node|course_presentation|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_location|taxonomy_term|provider|form';
  $field_group->group_name = 'group_location';
  $field_group->entity_type = 'taxonomy_term';
  $field_group->bundle = 'provider';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Location',
    'weight' => '3',
    'children' => array(
      0 => 'field_address',
      1 => 'field_email',
      2 => 'field_fax',
      3 => 'field_phone',
      4 => 'field_postcode',
      5 => 'field_town',
      6 => 'field_location_url',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => ' group-location field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_location|taxonomy_term|provider|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_location|taxonomy_term|venue|form';
  $field_group->group_name = 'group_location';
  $field_group->entity_type = 'taxonomy_term';
  $field_group->bundle = 'venue';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Location',
    'weight' => '4',
    'children' => array(
      0 => 'field_postcode',
      1 => 'field_town',
      2 => 'field_location_url',
      3 => 'field_address',
      4 => 'field_email',
      5 => 'field_fax',
      6 => 'field_phone',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => ' group-location field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_location|taxonomy_term|venue|form'] = $field_group;

  return $export;
}
