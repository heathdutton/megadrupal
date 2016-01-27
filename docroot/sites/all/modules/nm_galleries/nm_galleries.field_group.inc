<?php
/**
 * @file
 * nm_galleries.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function nm_galleries_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_nm_gallery_description|node|nm_gallery|form';
  $field_group->group_name = 'group_nm_gallery_description';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'nm_gallery';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_nm_gallery_info';
  $field_group->data = array(
    'label' => 'Gallery Description',
    'weight' => '17',
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
  $export['group_nm_gallery_description|node|nm_gallery|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_nm_gallery_images|node|nm_gallery|form';
  $field_group->group_name = 'group_nm_gallery_images';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'nm_gallery';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_nm_gallery_info';
  $field_group->data = array(
    'label' => 'Gallery Images',
    'weight' => '18',
    'children' => array(
      0 => 'field_nm_headline_image',
      1 => 'field_nm_gallery_images',
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
  $export['group_nm_gallery_images|node|nm_gallery|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_nm_gallery_info|node|nm_gallery|form';
  $field_group->group_name = 'group_nm_gallery_info';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'nm_gallery';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Gallery Information',
    'weight' => '1',
    'children' => array(
      0 => 'group_nm_gallery_description',
      1 => 'group_nm_gallery_images',
      2 => 'group_nm_gallery_relation',
      3 => 'group_nm_gallery_settings',
    ),
    'format_type' => 'tabs',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );
  $export['group_nm_gallery_info|node|nm_gallery|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_nm_gallery_relation|node|nm_gallery|form';
  $field_group->group_name = 'group_nm_gallery_relation';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'nm_gallery';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_nm_gallery_info';
  $field_group->data = array(
    'label' => 'Terms & Relation',
    'weight' => '19',
    'children' => array(
      0 => 'field_nm_tags',
      1 => 'field_nm_related_content',
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
  $export['group_nm_gallery_relation|node|nm_gallery|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_nm_gallery_settings|node|nm_gallery|form';
  $field_group->group_name = 'group_nm_gallery_settings';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'nm_gallery';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_nm_gallery_info';
  $field_group->data = array(
    'label' => 'Gallery Settings',
    'weight' => '20',
    'children' => array(
      0 => 'field_nm_gallery_embed_size',
      1 => 'field_nm_gallery_visibility',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Gallery Settings',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_nm_gallery_settings|node|nm_gallery|form'] = $field_group;

  return $export;
}
