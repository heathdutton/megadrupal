<?php
/**
 * @file
 * lessons_steps.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function lessons_steps_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_steps|node|lesson|form';
  $field_group->group_name = 'group_steps';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'lesson';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Steps / Instructions',
    'weight' => '5',
    'children' => array(
      0 => 'field_lesson_overview',
      1 => 'field_lesson_steps',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Steps / Instructions',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_steps|node|lesson|form'] = $field_group;

  return $export;
}
