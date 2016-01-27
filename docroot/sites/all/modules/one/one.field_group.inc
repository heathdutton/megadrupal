<?php
/**
 * @file
 * one.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function one_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_one_site_tags|node|one_site_content|form';
  $field_group->group_name = 'group_one_site_tags';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'one_site_content';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Add/Edit Tags',
    'weight' => '7',
    'children' => array(
      0 => 'field_one_site_grouped_by',
      1 => 'field_one_site_keywords',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => ' group-one-site-tags field-group-tab',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_one_site_tags|node|one_site_content|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_one_tab_link|node|one_site_content|form';
  $field_group->group_name = 'group_one_tab_link';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'one_site_content';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Add/Edit Link',
    'weight' => '5',
    'children' => array(
      0 => 'field_one_site_link',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => ' group-one-tab-link field-group-tab',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_one_tab_link|node|one_site_content|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_one_tab_media|node|one_site_content|form';
  $field_group->group_name = 'group_one_tab_media';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'one_site_content';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Add/Edit Media',
    'weight' => '2',
    'children' => array(
      0 => 'field_one_site_media',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Add/Edit Media',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => ' group-one-tab-media field-group-tab',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_one_tab_media|node|one_site_content|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_one_tab_text|node|one_site_content|form';
  $field_group->group_name = 'group_one_tab_text';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'one_site_content';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Add/Edit Text',
    'weight' => '1',
    'children' => array(
      0 => 'body',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Add/Edit Text',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => ' group-one-tab-text field-group-tab',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_one_tab_text|node|one_site_content|form'] = $field_group;

  return $export;
}
