<?php
/**
 * @file
 * drupress_blog.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function drupress_blog_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_blog_media|node|blog|form';
  $field_group->group_name = 'group_blog_media';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'blog';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Insert media',
    'weight' => '3',
    'children' => array(
      0 => 'field_blog_images',
      1 => 'field_blog_files',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Insert media',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => 'messages status',
        'description' => 'Use the fields below to upload images and files to the blog, then click in the editor where you want to insert the item, choose a display format and click the <em>Insert</em> button.',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_blog_media|node|blog|form'] = $field_group;

  return $export;
}
