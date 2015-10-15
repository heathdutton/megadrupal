<?php
/**
 * @file
 * Field group definitions for the PM Person module.
 */

/**
 * Implements hook_field_group_info().
 */
function pmperson_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_pm_info|user|user|default';
  $field_group->group_name = 'group_pm_info';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Project Management',
    'weight' => '0',
    'children' => array(
      0 => 'pm_description',
      1 => 'pmperson_im',
      2 => 'pmperson_name',
      3 => 'pmperson_phone',
      4 => 'pmperson_prefix',
      5 => 'pmperson_www',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-pm-info field-group-fieldset',
      ),
    ),
  );
  $export['group_pm_info|user|user|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_pm_info|user|user|form';
  $field_group->group_name = 'group_pm_info';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Project Management',
    'weight' => '3',
    'children' => array(
      0 => 'pm_description',
      1 => 'pmperson_im',
      2 => 'pmperson_name',
      3 => 'pmperson_phone',
      4 => 'pmperson_prefix',
      5 => 'pmperson_www',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-pm-info field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_pm_info|user|user|form'] = $field_group;

  return $export;
}
