<?php

/**
 * @file
 * Provides default property validation rules for breadcrumb path.
 */
 
/**
 * Implementation of hook_default_property_validation_rule().
 * 
 * Provide default validation rules.
 */
function breadcrumb2_default_property_validation_rule() {
  $export = array();

  $rule = new stdClass();
  $rule->disabled = FALSE; /* Edit this to true to make a default rule disabled initially */
  $rule->api_version = 2;
  $rule->rulename = 'Breadcrumb path unique';
  $rule->name = 'breadcrumb_path_unique';
  $rule->property_name = 'path';
  $rule->entity_type = 'breadcrumb2';
  $rule->bundle = 'breadcrumb2';
  $rule->validator = 'property_validation_unique_validator';
  $rule->settings = array(
    'data' => 'entity',
  );
  $rule->error_message = 'Breadcrumb path should be unique.';
  $export['breadcrumb_path_unique'] = $rule;
  
  $rule = new stdClass();
  $rule->disabled = FALSE; /* Edit this to true to make a default rule disabled initially */
  $rule->api_version = 2;
  $rule->rulename = 'Breadcrumb path valid path';
  $rule->name = 'breadcrumb_path_valid_path';
  $rule->property_name = 'path';
  $rule->entity_type = 'breadcrumb2';
  $rule->bundle = 'breadcrumb2';
  $rule->validator = 'property_validation_url_validator';
  $rule->settings = array(
    'external' => 0,
    'internal' => 1,
  );
  $rule->error_message = 'Breadcrumb path should be a valid path.';
  $export['breadcrumb_path_valid_path'] = $rule;
  
  return $export;
}
