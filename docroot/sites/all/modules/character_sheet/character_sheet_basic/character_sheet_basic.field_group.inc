<?php
/**
 * @file
 * character_sheet_basic.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function character_sheet_basic_field_group_info() {
  $export = array();

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_attributes|node|test_sheet|form';
  $field_group->group_name = 'group_attributes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Attributes',
    'weight' => '3',
    'children' => array(
      0 => 'field_str',
      1 => 'field_dex',
      2 => 'field_con',
      3 => 'field_int',
      4 => 'field_wis',
      5 => 'field_cha',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_attributes|node|test_sheet|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_skills|node|test_sheet|form';
  $field_group->group_name = 'group_skills';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Skills',
    'weight' => '4',
    'children' => array(
      0 => 'field_skill',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_skills|node|test_sheet|form'] = $field_group;

  return $export;
}
