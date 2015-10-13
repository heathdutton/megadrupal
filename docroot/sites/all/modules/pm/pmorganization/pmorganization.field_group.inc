<?php
/**
 * @file
 * Field group definitions for the PM Organization module.
 */

/**
 * Implements hook_field_group_info().
 */
function pmorganization_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_contact_info|node|pmorganization|default';
  $field_group->group_name = 'group_contact_info';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'pmorganization';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Contact Info',
    'weight' => '0',
    'children' => array(
      0 => 'pm_address',
      1 => 'pm_mail',
      2 => 'pm_phone',
      3 => 'pm_www',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-contact-info field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_contact_info|node|pmorganization|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_contact_info|node|pmorganization|form';
  $field_group->group_name = 'group_contact_info';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'pmorganization';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Contact Info',
    'weight' => '1',
    'children' => array(
      0 => 'pm_address',
      1 => 'pm_mail',
      2 => 'pm_phone',
      3 => 'pm_www',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-contact-info field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_contact_info|node|pmorganization|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_financial_info|node|pmorganization|default';
  $field_group->group_name = 'group_financial_info';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'pmorganization';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Financial Info',
    'weight' => '5',
    'children' => array(
      0 => 'pm_currency',
      1 => 'pm_price',
      2 => 'pm_pricemode',
      3 => 'pm_taxid',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-financial-info field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_financial_info|node|pmorganization|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_financial_info|node|pmorganization|form';
  $field_group->group_name = 'group_financial_info';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'pmorganization';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Financial Info',
    'weight' => '6',
    'children' => array(
      0 => 'pm_currency',
      1 => 'pm_price',
      2 => 'pm_pricemode',
      3 => 'pm_taxid',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-financial-info field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_financial_info|node|pmorganization|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_general_ino|node|pmorganization|default';
  $field_group->group_name = 'group_general_ino';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'pmorganization';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'General Info',
    'weight' => '10',
    'children' => array(
      0 => 'body',
      1 => 'pm_active',
      2 => 'pm_orglanguage',
      3 => 'pmorganization_type',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-general-ino field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_general_ino|node|pmorganization|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_general_ino|node|pmorganization|form';
  $field_group->group_name = 'group_general_ino';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'pmorganization';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'General Info',
    'weight' => '11',
    'children' => array(
      0 => 'body',
      1 => 'pm_active',
      2 => 'pm_orglanguage',
      3 => 'pmorganization_type',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-general-ino field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_general_ino|node|pmorganization|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_organization_members|node|pmorganization|default';
  $field_group->group_name = 'group_organization_members';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'pmorganization';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Organization members',
    'weight' => '14',
    'children' => array(
      0 => 'pmorganization_members',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-organization-members field-group-fieldset',
      ),
    ),
  );
  $export['group_organization_members|node|pmorganization|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_organization_members|node|pmorganization|form';
  $field_group->group_name = 'group_organization_members';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'pmorganization';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Organization members',
    'weight' => '15',
    'children' => array(
      0 => 'pmorganization_members',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-organization-members field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_organization_members|node|pmorganization|form'] = $field_group;

  return $export;
}
