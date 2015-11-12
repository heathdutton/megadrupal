<?php

/**
 * @file
 * Provide default field groups for module.
 */

/**
 * Implements hook_field_group_info().
 */
function dynamic_panes_fc_layout_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE;
  $field_group->api_version = 1;
  $field_group->identifier = 'group_additional|field_collection_item|field_dp_regions_blocks|form';
  $field_group->group_name = 'group_additional';
  $field_group->entity_type = 'field_collection_item';
  $field_group->bundle = 'field_dp_regions_blocks';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Additional settings',
    'weight' => '11',
    'children' => array(
      0 => 'field_dp_blocks_level',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Additional settings',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-additional field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_additional|field_collection_item|field_dp_regions_blocks|form'] = $field_group;

  return $export;
}
