<?php
/**
 * @file
 * nm_blog.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function nm_blog_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_nm_blog_info|node|nm_blog|form';
  $field_group->group_name = 'group_nm_blog_info';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'nm_blog';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Blog Information',
    'weight' => '0',
    'children' => array(
      0 => 'group_nm_blog_media',
      1 => 'group_nm_blog_post',
      2 => 'group_nm_blog_relations',
    ),
    'format_type' => 'tabs',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );
  $export['group_nm_blog_info|node|nm_blog|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_nm_blog_media|node|nm_blog|form';
  $field_group->group_name = 'group_nm_blog_media';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'nm_blog';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_nm_blog_info';
  $field_group->data = array(
    'label' => 'Media Assets',
    'weight' => '10',
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
  $export['group_nm_blog_media|node|nm_blog|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_nm_blog_post|node|nm_blog|form';
  $field_group->group_name = 'group_nm_blog_post';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'nm_blog';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_nm_blog_info';
  $field_group->data = array(
    'label' => 'Blog Data',
    'weight' => '9',
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
  $export['group_nm_blog_post|node|nm_blog|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_nm_blog_relations|node|nm_blog|form';
  $field_group->group_name = 'group_nm_blog_relations';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'nm_blog';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_nm_blog_info';
  $field_group->data = array(
    'label' => 'Terms & Relation',
    'weight' => '11',
    'children' => array(
      0 => 'field_nm_tags',
      1 => 'field_nm_related_content',
      2 => 'field_nm_blog_category',
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
  $export['group_nm_blog_relations|node|nm_blog|form'] = $field_group;

  return $export;
}
