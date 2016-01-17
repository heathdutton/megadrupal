<?php
/**
 * @file
 * cm_project_budget.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function cm_project_budget_field_group_info() {
  $export = array();

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
    'weight' => '5',
    'children' => array(
      0 => 'group_group',
      1 => 'field_project_budget',
      2 => 'field_deactivation_date',
      3 => 'field_reason_not_active',
      4 => 'field_affiliate_org',
      5 => 'field_department',
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
