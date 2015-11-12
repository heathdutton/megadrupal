<?php
/**
 * @file
 * Variable module integration with Grid Field Formatter.
 */

/**
 * Implements hook_variable_info().
 */
function grid_field_formatter_variable_info() {
  $variable = array();

  $variable['grid_field_formatter_field_types'] = array(
    'title' => t('Please select the field type for which Grid Field Formatter should be available.'),
    'type' => 'options',
    'options callback' => 'grid_field_formatter_field_types_options_list',
    'default' => array(),
    'group' => 'grid_field_formatter',
  );

  return $variable;
}

/**
 * Implements hook_variable_group_info().
 */
function grid_field_formatter_variable_group_info() {
  $groups['grid_field_formatter'] = array(
    'title' => t('Grid Field Formatter'),
    'description' => t('Grid Field Formatter configuration settings'),
    'access' => 'administer site configuration',
    'path' => array('admin/config/content/grid-field-formatter'),
  );
  return $groups;
}
