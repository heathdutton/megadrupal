<?php
/**
 * @file
 * mnn_project.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function mnn_project_field_group_info() {
  $export = array();

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_default_show_settings|node|cm_project|form';
  $field_group->group_name = 'group_default_show_settings';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'cm_project';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Default Show Settings',
    'weight' => '5',
    'children' => array(
      0 => 'field_format',
      1 => 'field_runtime',
      2 => 'field_pbcore_fcc_ratings',
      3 => 'field_pbcore_genres',
      4 => 'field_pbcore_languages',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Default Show Settings',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => 'New shows added to this Project will default to these settings.',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_default_show_settings|node|cm_project|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_legacy|node|cm_project|form';
  $field_group->group_name = 'group_legacy';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'cm_project';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Legacy',
    'weight' => '7',
    'children' => array(
      0 => 'field_project_id',
      1 => 'field_legacy_status',
      2 => 'field_project_msn',
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
  $export['group_legacy|node|cm_project|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_staff_only|node|cm_project|form';
  $field_group->group_name = 'group_staff_only';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'cm_project';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Staff Only',
    'weight' => '6',
    'children' => array(
      0 => 'group_group',
      1 => 'field_project_budget',
      2 => 'field_deactivation_date',
      3 => 'field_reason_not_active',
      4 => 'field_affiliate_org',
      5 => 'field_department',
      6 => 'field_device',
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
  $export['group_staff_only|node|cm_project|form'] = $field_group;

  return $export;
}
