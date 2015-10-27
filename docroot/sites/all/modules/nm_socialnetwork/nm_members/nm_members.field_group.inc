<?php
/**
 * @file
 * nm_members.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function nm_members_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_nm_bio|profile2|nm_member_profile|form';
  $field_group->group_name = 'group_nm_bio';
  $field_group->entity_type = 'profile2';
  $field_group->bundle = 'nm_member_profile';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_nm_profile';
  $field_group->data = array(
    'label' => 'Bio',
    'weight' => '2',
    'children' => array(
      0 => 'field_nm_bio',
      1 => 'field_nm_my_website',
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
  $export['group_nm_bio|profile2|nm_member_profile|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_nm_image_networks|profile2|nm_member_profile|form';
  $field_group->group_name = 'group_nm_image_networks';
  $field_group->entity_type = 'profile2';
  $field_group->bundle = 'nm_member_profile';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_nm_networks';
  $field_group->data = array(
    'label' => 'Image Networks',
    'weight' => '19',
    'children' => array(
      0 => 'field_nm_flickr',
      1 => 'field_nm_instagram',
      2 => 'field_nm_pinterest',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Image Networks',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_nm_image_networks|profile2|nm_member_profile|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_nm_name|profile2|nm_member_profile|form';
  $field_group->group_name = 'group_nm_name';
  $field_group->entity_type = 'profile2';
  $field_group->bundle = 'nm_member_profile';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_nm_profile';
  $field_group->data = array(
    'label' => 'Name',
    'weight' => '0',
    'children' => array(
      0 => 'field_nm_first_name',
      1 => 'field_nm_last_name',
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
  $export['group_nm_name|profile2|nm_member_profile|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_nm_networks|profile2|nm_member_profile|form';
  $field_group->group_name = 'group_nm_networks';
  $field_group->entity_type = 'profile2';
  $field_group->bundle = 'nm_member_profile';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_nm_profile';
  $field_group->data = array(
    'label' => 'External Networks',
    'weight' => '3',
    'children' => array(
      0 => 'group_nm_video_networks',
      1 => 'group_nm_professional_networks',
      2 => 'group_nm_social_networks',
      3 => 'group_nm_image_networks',
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
  $export['group_nm_networks|profile2|nm_member_profile|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_nm_professional_networks|profile2|nm_member_profile|form';
  $field_group->group_name = 'group_nm_professional_networks';
  $field_group->entity_type = 'profile2';
  $field_group->bundle = 'nm_member_profile';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_nm_networks';
  $field_group->data = array(
    'label' => 'Professional Networks',
    'weight' => '18',
    'children' => array(
      0 => 'field_nm_linkedin',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Professional Networks',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_nm_professional_networks|profile2|nm_member_profile|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_nm_profile_images|profile2|nm_member_profile|form';
  $field_group->group_name = 'group_nm_profile_images';
  $field_group->entity_type = 'profile2';
  $field_group->bundle = 'nm_member_profile';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_nm_profile';
  $field_group->data = array(
    'label' => 'Images',
    'weight' => '1',
    'children' => array(
      0 => 'field_nm_cover_photo',
      1 => 'field_nm_profile_picture',
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
  $export['group_nm_profile_images|profile2|nm_member_profile|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_nm_profile|profile2|nm_member_profile|form';
  $field_group->group_name = 'group_nm_profile';
  $field_group->entity_type = 'profile2';
  $field_group->bundle = 'nm_member_profile';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Profile Information',
    'weight' => '1',
    'children' => array(
      0 => 'group_nm_networks',
      1 => 'group_nm_bio',
      2 => 'group_nm_profile_images',
      3 => 'group_nm_name',
    ),
    'format_type' => 'tabs',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );
  $export['group_nm_profile|profile2|nm_member_profile|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_nm_social_networks|profile2|nm_member_profile|form';
  $field_group->group_name = 'group_nm_social_networks';
  $field_group->entity_type = 'profile2';
  $field_group->bundle = 'nm_member_profile';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_nm_networks';
  $field_group->data = array(
    'label' => 'Social Networks',
    'weight' => '17',
    'children' => array(
      0 => 'field_nm_twitter',
      1 => 'field_nm_facebook',
      2 => 'field_nm_googleplus',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Social Networks',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_nm_social_networks|profile2|nm_member_profile|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_nm_video_networks|profile2|nm_member_profile|form';
  $field_group->group_name = 'group_nm_video_networks';
  $field_group->entity_type = 'profile2';
  $field_group->bundle = 'nm_member_profile';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_nm_networks';
  $field_group->data = array(
    'label' => 'Video Networks',
    'weight' => '20',
    'children' => array(
      0 => 'field_nm_youtube',
      1 => 'field_nm_vimeo',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Video Networks',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_nm_video_networks|profile2|nm_member_profile|form'] = $field_group;

  return $export;
}
