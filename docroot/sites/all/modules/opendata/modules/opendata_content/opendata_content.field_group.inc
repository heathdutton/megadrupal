<?php
/**
 * @file
 * opendata_content.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function opendata_content_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_opendata_core_if_app|node|opendata_dataset|form';
  $field_group->group_name = 'group_opendata_core_if_app';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'opendata_dataset';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Common Core (Required if Applicable)',
    'weight' => '1',
    'children' => array(
      0 => 'field_opendata_data_dictionary',
      1 => 'field_opendata_distribution',
      2 => 'field_opendata_license',
      3 => 'field_opendata_spatial',
      4 => 'field_opendata_temporal',
      5 => 'field_opendata_web_service',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Common Core (Required if Applicable)',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_opendata_core_if_app|node|opendata_dataset|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_opendata_core|node|opendata_dataset|form';
  $field_group->group_name = 'group_opendata_core';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'opendata_dataset';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Common Core (Required)',
    'weight' => '0',
    'children' => array(
      0 => 'field_opendata_access_level',
      1 => 'field_opendata_description',
      2 => 'field_opendata_identifier',
      3 => 'field_opendata_keyword',
      4 => 'field_opendata_mbox',
      5 => 'field_opendata_modified',
      6 => 'field_opendata_person',
      7 => 'field_opendata_publisher',
      8 => 'title',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Common Core (Required)',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_opendata_core|node|opendata_dataset|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_opendata_expanded|node|opendata_dataset|form';
  $field_group->group_name = 'group_opendata_expanded';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'opendata_dataset';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Expanded Fields',
    'weight' => '2',
    'children' => array(
      0 => 'field_opendata_data_quality',
      1 => 'field_opendata_feed',
      2 => 'field_opendata_frequency',
      3 => 'field_opendata_granularity',
      4 => 'field_opendata_landing_page',
      5 => 'field_opendata_language',
      6 => 'field_opendata_references',
      7 => 'field_opendata_release_date',
      8 => 'field_opendata_size',
      9 => 'field_opendata_system_of_records',
      10 => 'field_opendata_theme',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Expanded Fields',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_opendata_expanded|node|opendata_dataset|form'] = $field_group;

  return $export;
}
