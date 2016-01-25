<?php
/**
 * @file
 * one_import.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function one_import_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_one_tab_import|node|one_site_content|form';
  $field_group->group_name = 'group_one_tab_import';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'one_site_content';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Import',
    'weight' => '8',
    'children' => array(
      0 => 'feeds',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_one_tab_import|node|one_site_content|form'] = $field_group;

  return $export;
}
