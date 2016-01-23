<?php
/**
 * @file
 * cms_blog.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function cms_blog_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_cms_blog_content_pane|node|blog|teaser';
  $field_group->group_name = 'group_cms_blog_content_pane';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'blog';
  $field_group->mode = 'teaser';
  $field_group->parent_name = 'group_cms_blog_row';
  $field_group->data = array(
    'label' => 'Blog Listing Content Pane',
    'weight' => '12',
    'children' => array(
      0 => 'body',
      1 => 'field_blog_category',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Blog Listing Content Pane',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-cms-blog-content-pane col-sm-8',
        'description' => '',
        'show_label' => '0',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_cms_blog_content_pane|node|blog|teaser'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_cms_blog_row|node|blog|teaser';
  $field_group->group_name = 'group_cms_blog_row';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'blog';
  $field_group->mode = 'teaser';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Blog Listing Row',
    'weight' => '0',
    'children' => array(
      0 => 'field_blog_image',
      1 => 'group_cms_blog_content_pane',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Blog Listing Row',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-cms-blog-row',
        'description' => '',
        'show_label' => '0',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_cms_blog_row|node|blog|teaser'] = $field_group;

  return $export;
}
