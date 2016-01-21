<?php
/**
 * @file
 * ns_field_translation_test.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function ns_field_translation_test_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_ns_field_translation_test';
  $strongarm->value = '0';
  $export['language_content_type_ns_field_translation_test'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_ns_field_translation_test';
  $strongarm->value = array(
    'main-menu' => 'main-menu',
    'devel' => 0,
    'management' => 0,
    'navigation' => 0,
    'user-menu' => 0,
  );
  $export['menu_options_ns_field_translation_test'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_ns_field_translation_test';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_ns_field_translation_test'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_ns_field_translation_test';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );
  $export['node_options_ns_field_translation_test'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_ns_field_translation_test';
  $strongarm->value = '1';
  $export['node_preview_ns_field_translation_test'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_ns_field_translation_test';
  $strongarm->value = 1;
  $export['node_submitted_ns_field_translation_test'] = $strongarm;

  return $export;
}
