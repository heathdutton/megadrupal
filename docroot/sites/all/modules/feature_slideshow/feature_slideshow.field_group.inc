<?php
/**
 * @file
 * feature_slideshow.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function feature_slideshow_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_page_slideshow|node|page|form';
  $field_group->group_name = 'group_page_slideshow';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'page';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Slideshow',
    'weight' => '5',
    'children' => array(
      0 => 'field_page_slideshow',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-page-slideshow field-group-tab',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_page_slideshow|node|page|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_slide_content|node|slide|form';
  $field_group->group_name = 'group_slide_content';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'slide';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Content',
    'weight' => '7',
    'children' => array(
      0 => 'field_slide_subtitle',
      1 => 'field_slide_link',
      2 => 'field_slide_image',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-slide-content field-group-tab',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_slide_content|node|slide|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_slide_style|node|slide|form';
  $field_group->group_name = 'group_slide_style';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'slide';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Style',
    'weight' => '9',
    'children' => array(
      0 => 'field_slide_style',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-slide-style field-group-tab',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_slide_style|node|slide|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_slideshow_options|node|slideshow|form';
  $field_group->group_name = 'group_slideshow_options';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'slideshow';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Slideshow Options',
    'weight' => '4',
    'children' => array(
      0 => 'field_slideshow_controls',
      1 => 'field_slideshow_pager',
      2 => 'field_slideshow_speed',
      3 => 'field_slideshow_timeout',
      4 => 'field_slideshow_transition',
      5 => 'field_slideshow_automatic',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-slideshow-options field-group-tab',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_slideshow_options|node|slideshow|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_slideshow_slides|node|slideshow|form';
  $field_group->group_name = 'group_slideshow_slides';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'slideshow';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Slides',
    'weight' => '3',
    'children' => array(
      0 => 'field_slideshow_slides',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Slides',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-slideshow-slides field-group-tab',
        'description' => '',
      ),
      'formatter' => 'closed',
    ),
  );
  $export['group_slideshow_slides|node|slideshow|form'] = $field_group;

  return $export;
}
