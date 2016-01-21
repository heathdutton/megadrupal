<?php
/**
 * @file
 * advanced_page.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function advanced_page_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_file_attachments|node|advanced_page|form';
  $field_group->group_name = 'group_file_attachments';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'advanced_page';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Attachments',
    'weight' => '3',
    'children' => array(
      0 => 'field_advanced_page_attach_css',
      1 => 'field_advanced_page_attach_file',
      2 => 'field_advanced_page_attach_js',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Attachments',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_file_attachments|node|advanced_page|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_page_status|node|advanced_page|form';
  $field_group->group_name = 'group_page_status';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'advanced_page';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Page Status',
    'weight' => '4',
    'children' => array(
      0 => 'field_advanced_page_status',
      1 => 'field_advanced_page_url',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Page Status',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_page_status|node|advanced_page|form'] = $field_group;

  return $export;
}
