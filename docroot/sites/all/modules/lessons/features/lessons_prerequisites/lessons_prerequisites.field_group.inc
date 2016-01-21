<?php
/**
 * @file
 * lessons_prerequisites.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function lessons_prerequisites_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_prerequisites|node|lesson|form';
  $field_group->group_name = 'group_prerequisites';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'lesson';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Prerequisites',
    'weight' => '5',
    'children' => array(
      0 => 'field_lesson_prerequisites',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Prerequisites',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_prerequisites|node|lesson|form'] = $field_group;

  return $export;
}
