<?php
/**
 * @file
 * one_date.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function one_date_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_one_tab_date|node|one_site_content|form';
  $field_group->group_name = 'group_one_tab_date';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'one_site_content';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Add/Edit Date',
    'weight' => '3',
    'children' => array(
      0 => 'field_one_site_date_partial',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Add/Edit Date',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'closed',
    ),
  );
  $export['group_one_tab_date|node|one_site_content|form'] = $field_group;

  return $export;
}
