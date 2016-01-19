<?php
/**
 * @file
 * drush_recipes_entity.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function drush_recipes_entity_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_metadata|drush_recipes|recipe|default';
  $field_group->group_name = 'group_metadata';
  $field_group->entity_type = 'drush_recipes';
  $field_group->bundle = 'recipe';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Metadata',
    'weight' => '3',
    'children' => array(
      0 => 'field_type',
      1 => 'field_description',
      2 => 'field_version',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Metadata',
      'instance_settings' => array(
        'classes' => 'group-metadata field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_metadata|drush_recipes|recipe|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_metadata|drush_recipes|recipe|drush_recipes_xml_output';
  $field_group->group_name = 'group_metadata';
  $field_group->entity_type = 'drush_recipes';
  $field_group->bundle = 'recipe';
  $field_group->mode = 'drush_recipes_xml_output';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Metadata',
    'weight' => '2',
    'children' => array(
      0 => 'field_type',
      1 => 'field_description',
      2 => 'field_version',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-metadata field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_metadata|drush_recipes|recipe|drush_recipes_xml_output'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_metadata|drush_recipes|recipe|form';
  $field_group->group_name = 'group_metadata';
  $field_group->entity_type = 'drush_recipes';
  $field_group->bundle = 'recipe';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Metadata',
    'weight' => '4',
    'children' => array(
      0 => 'field_type',
      1 => 'field_description',
      2 => 'field_version',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-metadata field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_metadata|drush_recipes|recipe|form'] = $field_group;

  return $export;
}
