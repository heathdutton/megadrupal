<?php
/**
 * @file
 * lessons_info.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function lessons_info_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_lesson_info|node|lesson|form';
  $field_group->group_name = 'group_lesson_info';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'lesson';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Lesson info',
    'weight' => '4',
    'children' => array(
      0 => 'field_lesson_drupal_version',
      1 => 'field_lesson_project_name',
      2 => 'field_lesson_project_type',
      3 => 'field_lesson_type',
      4 => 'field_lesson_tags',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Lesson info',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_lesson_info|node|lesson|form'] = $field_group;

  return $export;
}
