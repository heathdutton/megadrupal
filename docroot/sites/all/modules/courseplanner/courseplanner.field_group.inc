<?php
/**
 * @file
 * courseplanner.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function courseplanner_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_cp_metadata|node|cp_offering|form';
  $field_group->group_name = 'group_cp_metadata';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'cp_offering';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Metadata',
    'weight' => '4',
    'children' => array(
      0 => 'cp_canonical_course',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Metadata',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => ' group-cp-metadata field-group-tab',
        'description' => 'Adding metadata will others find this item, and understand when it is useful.',
      ),
      'formatter' => 'closed',
    ),
  );
  $export['group_cp_metadata|node|cp_offering|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_cp_metadata|node|cp_outline|form';
  $field_group->group_name = 'group_cp_metadata';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'cp_outline';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Metadata',
    'weight' => '6',
    'children' => array(
      0 => 'cp_canonical_course',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Metadata',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => ' group-cp-metadata field-group-tab',
        'description' => 'Adding metadata will others find this item, and understand when it is useful.',
      ),
      'formatter' => 'closed',
    ),
  );
  $export['group_cp_metadata|node|cp_outline|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_cp_metadata|node|cp_resource|form';
  $field_group->group_name = 'group_cp_metadata';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'cp_resource';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Metadata',
    'weight' => '8',
    'children' => array(
      0 => 'cp_canonical_course',
      1 => 'cp_keywords',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Metadata',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => ' group-cp-metadata field-group-tab',
        'description' => 'Adding metadata will others find this item, and understand when it is useful.',
      ),
      'formatter' => 'closed',
    ),
  );
  $export['group_cp_metadata|node|cp_resource|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_cp_metadata|node|cp_section|form';
  $field_group->group_name = 'group_cp_metadata';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'cp_section';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Metadata',
    'weight' => '4',
    'children' => array(
      0 => 'cp_canonical_course',
      1 => 'cp_keywords',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Metadata',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => ' group-cp-metadata field-group-tab',
        'description' => 'Adding metadata will others find this item, and understand when it is useful.',
      ),
      'formatter' => 'closed',
    ),
  );
  $export['group_cp_metadata|node|cp_section|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_cp_resource|node|cp_resource|form';
  $field_group->group_name = 'group_cp_resource';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'cp_resource';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Resource',
    'weight' => '1',
    'children' => array(
      0 => 'cp_resource_file',
      1 => 'cp_resource_link',
      2 => 'cp_resource_reference',
      3 => 'cp_resource_source',
      4 => 'cp_resource_type',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => ' group-cp-resource field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_cp_resource|node|cp_resource|form'] = $field_group;

  return $export;
}
