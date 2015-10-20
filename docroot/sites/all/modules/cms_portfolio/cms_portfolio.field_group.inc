<?php
/**
 * @file
 * cms_portfolio.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function cms_portfolio_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_cms_portfolio_fields|node|portfolio|default';
  $field_group->group_name = 'group_cms_portfolio_fields';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'portfolio';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Portfolio Extra Fields',
    'weight' => '3',
    'children' => array(
      0 => 'field_cms_portfolio_client',
      1 => 'field_cms_portoflio_custom',
      2 => 'field_cms_portfolio_links',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Portfolio Extra Fields',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-portfolio-extra field-group-div col-sm-4 clear-both',
        'description' => '',
        'show_label' => '0',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_cms_portfolio_fields|node|portfolio|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_cms_portfolio_fields|node|portfolio|form';
  $field_group->group_name = 'group_cms_portfolio_fields';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'portfolio';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Portfolio extra fields',
    'weight' => '4',
    'children' => array(
      0 => 'field_cms_portfolio_client',
      1 => 'field_cms_portoflio_custom',
      2 => 'field_cms_portfolio_links',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-cms-portfolio-fields field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_cms_portfolio_fields|node|portfolio|form'] = $field_group;

  return $export;
}
