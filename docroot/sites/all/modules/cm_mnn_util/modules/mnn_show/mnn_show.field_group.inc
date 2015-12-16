<?php
/**
 * @file
 * mnn_show.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function mnn_show_field_group_info() {
  $export = array();

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_physical_media_info|node|cm_show|form';
  $field_group->group_name = 'group_physical_media_info';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'cm_show';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Tape Library Information',
    'weight' => '9',
    'children' => array(
      0 => 'field_delivered_via',
      1 => 'field_reject_reason',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Tape Library Information',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_physical_media_info|node|cm_show|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_show_basic_information|node|cm_show|form';
  $field_group->group_name = 'group_show_basic_information';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'cm_show';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Basic Information',
    'weight' => '2',
    'children' => array(
      0 => 'field_format',
      1 => 'taxonomy_cm_project_types',
      2 => 'field_device',
      3 => 'field_runtime',
      4 => 'field_narrative',
      5 => 'field_show_notes',
      6 => 'field_short_description',
      7 => 'field_episode_number',
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
  $export['group_show_basic_information|node|cm_show|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_show_categories|node|cm_show|form';
  $field_group->group_name = 'group_show_categories';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'cm_show';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Categories',
    'weight' => '10',
    'children' => array(
      0 => 'field_project_audience',
      1 => 'field_topic',
      2 => 'field_pbcore_fcc_ratings',
      3 => 'field_pbcore_genres',
      4 => 'field_pbcore_languages',
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
  $export['group_show_categories|node|cm_show|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_show_producer_info|node|cm_show|form';
  $field_group->group_name = 'group_show_producer_info';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'cm_show';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Special Producer Information',
    'weight' => '12',
    'children' => array(
      0 => 'field_producer_name',
      1 => 'field_producer_address',
      2 => 'field_producer_phone',
      3 => 'field_producer_email',
      4 => 'field_producer_website',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Special Producer Information',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_show_producer_info|node|cm_show|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_timing_information|node|cm_show|form';
  $field_group->group_name = 'group_timing_information';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'cm_show';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Timing Information',
    'weight' => '11',
    'children' => array(
      0 => 'field_timecode_in',
      1 => 'field_timecode_out',
      2 => 'field_trans_timecode_in',
      3 => 'field_trans_timecode_out',
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
  $export['group_timing_information|node|cm_show|form'] = $field_group;

  return $export;
}
