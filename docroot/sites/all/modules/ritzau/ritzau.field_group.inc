<?php
/**
 * @file
 * ritzau.field_group.inc
 */


/**
 * Implements hook_field_group_info().
 */
function ritzau_field_group_info() {
  $export = array();

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_metadata|node|ritzau|form';
  $field_group->group_name = 'group_metadata';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'ritzau';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Ritzau metadata',
    'weight' => '6',
    'children' => array(
      0 => 'field_ritzau_sent_time',
      1 => 'field_ritzau_original_sent_time',
      2 => 'field_ritzau_release_time',
      3 => 'field_ritzau_id',
      4 => 'field_ritzau_refering_id',
      5 => 'field_ritzau_source',
      6 => 'field_ritzau_urgency',
      7 => 'field_ritzau_updated',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Ritzau metadata',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_metadata|node|wire|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_taxonomy|node|ritzau|form';
  $field_group->group_name = 'group_taxonomy';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'ritzau';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Ritzau taxonomy',
    'weight' => '3',
    'children' => array(
      0 => 'field_ritzau_iptc_subject',
      1 => 'field_ritzau_genre',
      2 => 'field_ritzau_category',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Ritzau taxonomy',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_taxonomy|node|wire|form'] = $field_group;

  return $export;
}
