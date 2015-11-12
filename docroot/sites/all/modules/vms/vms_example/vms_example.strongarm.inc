<?php
/**
 * @file
 * vms_example.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function vms_example_strongarm() {
  $export = array();

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_sidebar_message';
  $strongarm->value = 0;
  $export['comment_anonymous_sidebar_message'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_sidebar_message';
  $strongarm->value = 1;
  $export['comment_default_mode_sidebar_message'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_sidebar_message';
  $strongarm->value = '50';
  $export['comment_default_per_page_sidebar_message'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_sidebar_message';
  $strongarm->value = 1;
  $export['comment_form_location_sidebar_message'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_sidebar_message';
  $strongarm->value = '1';
  $export['comment_preview_sidebar_message'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_sidebar_message';
  $strongarm->value = '1';
  $export['comment_sidebar_message'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_sidebar_message';
  $strongarm->value = 1;
  $export['comment_subject_field_sidebar_message'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_sidebar_message';
  $strongarm->value = array();
  $export['menu_options_sidebar_message'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_sidebar_message';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_sidebar_message'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_sidebar_message';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_sidebar_message'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_sidebar_message';
  $strongarm->value = 0;
  $export['node_submitted_sidebar_message'] = $strongarm;

  return $export;
}
