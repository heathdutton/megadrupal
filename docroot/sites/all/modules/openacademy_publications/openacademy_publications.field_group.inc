<?php
/**
 * @file
 * openacademy_publications.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function openacademy_publications_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_publication_information|node|openacademy_publication|form';
  $field_group->group_name = 'group_publication_information';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'openacademy_publication';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Publishing Information',
    'weight' => '2',
    'children' => array(
      0 => 'field_publication_author',
      1 => 'field_publication_action',
      2 => 'field_publication_published',
      3 => 'field_publication_publisher',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Publishing Information',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_publication_information|node|openacademy_publication|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_publishing_details|node|openacademy_publication|form';
  $field_group->group_name = 'group_publishing_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'openacademy_publication';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Publishing Details',
    'weight' => '3',
    'children' => array(
      0 => 'body',
      1 => 'field_publication_citation',
      2 => 'field_tags',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Publishing Details',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_publishing_details|node|openacademy_publication|form'] = $field_group;

  return $export;
}
