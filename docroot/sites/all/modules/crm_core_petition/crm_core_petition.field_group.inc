<?php
/**
 * @file
 * crm_core_petition.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function crm_core_petition_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_cmcp_personalize|node|cmcp_petition|form';
  $field_group->group_name = 'group_cmcp_personalize';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'cmcp_petition';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Personalized Message',
    'weight' => '6',
    'children' => array(
      0 => 'field_cmcp_personal_message',
      1 => 'field_cmcp_pm_customize',
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
  $export['group_cmcp_personalize|node|cmcp_petition|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_cmcp_send_target|node|cmcp_petition|form';
  $field_group->group_name = 'group_cmcp_send_target';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'cmcp_petition';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Petition targets',
    'weight' => '35',
    'children' => array(
      0 => 'field_cmcp_target_emails',
      1 => 'field_cmcp_target_ids',
      2 => 'field_cmcp_target_email_subject',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Petition targets',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'closed',
    ),
  );
  $export['group_cmcp_send_target|node|cmcp_petition|form'] = $field_group;

  return $export;
}
