<?php
/**
 * @file
 * nm_testimonials.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function nm_testimonials_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_nm_testimonial_info|node|nm_testimonial|form';
  $field_group->group_name = 'group_nm_testimonial_info';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'nm_testimonial';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Testimonial Information',
    'weight' => '5',
    'children' => array(
      0 => 'group_nm_testimonial_rating',
      1 => 'group_nm_testimonial_user',
    ),
    'format_type' => 'tabs',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );
  $export['group_nm_testimonial_info|node|nm_testimonial|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_nm_testimonial_rating|node|nm_testimonial|form';
  $field_group->group_name = 'group_nm_testimonial_rating';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'nm_testimonial';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_nm_testimonial_info';
  $field_group->data = array(
    'label' => 'Rating',
    'weight' => '6',
    'children' => array(
      0 => 'field_nm_body',
      1 => 'field_nm_rate',
      2 => 'title',
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
  $export['group_nm_testimonial_rating|node|nm_testimonial|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_nm_testimonial_user|node|nm_testimonial|form';
  $field_group->group_name = 'group_nm_testimonial_user';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'nm_testimonial';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_nm_testimonial_info';
  $field_group->data = array(
    'label' => 'Your Info',
    'weight' => '7',
    'children' => array(
      0 => 'field_nm_email',
      1 => 'field_nm_name',
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
  $export['group_nm_testimonial_user|node|nm_testimonial|form'] = $field_group;

  return $export;
}
