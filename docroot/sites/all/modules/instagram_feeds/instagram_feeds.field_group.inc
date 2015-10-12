<?php
/**
 * @file
 * instagram_feeds.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function instagram_feeds_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_div|node|instagram_feed|form';
  $field_group->group_name = 'group_div';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'instagram_feed';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_thumbnail_settings';
  $field_group->data = array(
    'label' => 'Wrapper',
    'weight' => '8',
    'children' => array(
      0 => 'field_instf_thumb_size',
      1 => 'group_thumbnail_border',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Wrapper',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => 'instagram-form-wrapper clearfix',
        'description' => '',
        'show_label' => '0',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'none',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_div|node|instagram_feed|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_feed_dimensions|node|instagram_feed|form';
  $field_group->group_name = 'group_feed_dimensions';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'instagram_feed';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_thumbnail_settings';
  $field_group->data = array(
    'label' => 'Feed Dimensions',
    'weight' => '6',
    'children' => array(
      0 => 'field_instf_columns',
      1 => 'field_instf_rows',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Feed Dimensions',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'instagram-form-wrapper clearfix',
        'description' => '',
        'show_label' => '1',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_feed_dimensions|node|instagram_feed|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_image|node|instagram_media_item|form';
  $field_group->group_name = 'group_image';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'instagram_media_item';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Image Data',
    'weight' => '0',
    'children' => array(
      0 => 'body',
      1 => 'field_instf_comments_count',
      2 => 'field_instf_created',
      3 => 'field_instf_hash_tags',
      4 => 'field_instf_id',
      5 => 'field_instf_like_count',
      6 => 'field_instf_link_url',
      7 => 'field_instf_marker',
      8 => 'field_instf_uid',
      9 => 'field_instf_user',
      10 => 'field_instf_image_url',
      11 => 'field_instf_thumb_url',
      12 => 'title',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Image Data',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_image|node|instagram_media_item|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_instagram_feed_type|node|instagram_feed|form';
  $field_group->group_name = 'group_instagram_feed_type';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'instagram_feed';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_settings';
  $field_group->data = array(
    'label' => 'Wrapper',
    'weight' => '6',
    'children' => array(
      0 => 'field_instf_favorites',
      1 => 'title',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Wrapper',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => 'clearfix instagram-form-wrapper',
        'description' => '',
        'show_label' => '0',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_instagram_feed_type|node|instagram_feed|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_instagram_sources|node|instagram_feed|form';
  $field_group->group_name = 'group_instagram_sources';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'instagram_feed';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_settings';
  $field_group->data = array(
    'label' => 'Instagram Sources',
    'weight' => '7',
    'children' => array(
      0 => 'field_instf_hash_tags',
      1 => 'field_instf_user',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Instagram Sources',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_instagram_sources|node|instagram_feed|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_instagram_tabs|node|instagram_feed|form';
  $field_group->group_name = 'group_instagram_tabs';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'instagram_feed';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Instagram TABS',
    'weight' => '0',
    'children' => array(
      0 => 'group_settings',
      1 => 'group_style',
    ),
    'format_type' => 'htabs',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );
  $export['group_instagram_tabs|node|instagram_feed|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_popup_settings|node|instagram_feed|form';
  $field_group->group_name = 'group_popup_settings';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'instagram_feed';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_style';
  $field_group->data = array(
    'label' => 'Popup image settings',
    'weight' => '20',
    'children' => array(
      0 => 'field_instf_display_on_popup',
      1 => 'group_wrapper',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Popup image settings',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'clearfix',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_popup_settings|node|instagram_feed|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_settings|node|instagram_feed|form';
  $field_group->group_name = 'group_settings';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'instagram_feed';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_instagram_tabs';
  $field_group->data = array(
    'label' => 'Manage Feed Settings',
    'weight' => '1',
    'children' => array(
      0 => 'field_instf_marker',
      1 => 'group_instagram_feed_type',
      2 => 'group_instagram_sources',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_settings|node|instagram_feed|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_style|node|instagram_feed|form';
  $field_group->group_name = 'group_style';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'instagram_feed';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_instagram_tabs';
  $field_group->data = array(
    'label' => 'Manage Feed Style',
    'weight' => '3',
    'children' => array(
      0 => 'group_popup_settings',
      1 => 'group_thumbnail_settings',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_style|node|instagram_feed|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_thumbnail_border|node|instagram_feed|form';
  $field_group->group_name = 'group_thumbnail_border';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'instagram_feed';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_div';
  $field_group->data = array(
    'label' => 'Thumbnail Border',
    'weight' => '8',
    'children' => array(
      0 => 'field_instf_border_width',
      1 => 'field_instf_color',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Thumbnail Border',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'instagram-form-wrapper clearfix',
        'description' => '',
        'show_label' => '1',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_thumbnail_border|node|instagram_feed|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_thumbnail_settings|node|instagram_feed|form';
  $field_group->group_name = 'group_thumbnail_settings';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'instagram_feed';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_style';
  $field_group->data = array(
    'label' => 'Thumbnail Settings',
    'weight' => '19',
    'children' => array(
      0 => 'group_div',
      1 => 'group_feed_dimensions',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Thumbnail Settings',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_thumbnail_settings|node|instagram_feed|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_wrapper|node|instagram_feed|form';
  $field_group->group_name = 'group_wrapper';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'instagram_feed';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_popup_settings';
  $field_group->data = array(
    'label' => 'Wrapper',
    'weight' => '29',
    'children' => array(
      0 => 'field_instf_font_color',
      1 => 'field_instf_font_family',
      2 => 'field_instf_font_size',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Wrapper',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'instagram-form-wrapper clearfix float-left',
        'description' => '',
        'show_label' => '0',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_wrapper|node|instagram_feed|form'] = $field_group;

  return $export;
}
