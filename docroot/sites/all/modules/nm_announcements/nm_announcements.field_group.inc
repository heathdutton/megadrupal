<?php
/**
 * @file
 * nm_announcements.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function nm_announcements_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_nm_announcement_info|node|nm_announcement|form';
  $field_group->group_name = 'group_nm_announcement_info';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'nm_announcement';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Announcement Information',
    'weight' => '0',
    'children' => array(
      0 => 'group_nm_announcement_media',
      1 => 'group_nm_announcement_post',
      2 => 'group_nm_announcement_relations',
    ),
    'format_type' => 'tabs',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );
  $export['group_nm_announcement_info|node|nm_announcement|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_nm_announcement_media|node|nm_announcement|form';
  $field_group->group_name = 'group_nm_announcement_media';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'nm_announcement';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_nm_announcement_info';
  $field_group->data = array(
    'label' => 'Media Assets',
    'weight' => '36',
    'children' => array(
      0 => 'field_nm_headline_image',
      1 => 'field_nm_attach_gallery',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_nm_announcement_media|node|nm_announcement|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_nm_announcement_post|node|nm_announcement|form';
  $field_group->group_name = 'group_nm_announcement_post';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'nm_announcement';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_nm_announcement_info';
  $field_group->data = array(
    'label' => 'Announcement Data',
    'weight' => '35',
    'children' => array(
      0 => 'field_nm_body',
      1 => 'title',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_nm_announcement_post|node|nm_announcement|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_nm_announcement_relations|node|nm_announcement|form';
  $field_group->group_name = 'group_nm_announcement_relations';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'nm_announcement';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_nm_announcement_info';
  $field_group->data = array(
    'label' => 'Terms & Relation',
    'weight' => '37',
    'children' => array(
      0 => 'field_nm_tags',
      1 => 'field_nm_related_content',
      2 => 'field_nm_announcement_category',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_nm_announcement_relations|node|nm_announcement|form'] = $field_group;

  return $export;
}
