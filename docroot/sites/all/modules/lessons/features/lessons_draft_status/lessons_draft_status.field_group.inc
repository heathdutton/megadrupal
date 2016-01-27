<?php
/**
 * @file
 * lessons_draft_status.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function lessons_draft_status_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_draft_status|node|lesson|form';
  $field_group->group_name = 'group_draft_status';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'lesson';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Draft status',
    'weight' => '2',
    'children' => array(
      0 => 'field_lesson_draft_status',
      1 => 'field_lesson_last_peer_review',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Draft status',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_draft_status|node|lesson|form'] = $field_group;

  return $export;
}
