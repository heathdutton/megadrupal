<?php
/**
 * @file
 * ns_footer_content.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function ns_footer_content_defaultconfig_strongarm() {
  $export = array();

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_ns_footer_content';
  $strongarm->value = 0;
  $export['comment_anonymous_ns_footer_content'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_ns_footer_content';
  $strongarm->value = 1;
  $export['comment_default_mode_ns_footer_content'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_ns_footer_content';
  $strongarm->value = '50';
  $export['comment_default_per_page_ns_footer_content'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_ns_footer_content';
  $strongarm->value = 1;
  $export['comment_form_location_ns_footer_content'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_ns_footer_content';
  $strongarm->value = '0';
  $export['comment_ns_footer_content'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_ns_footer_content';
  $strongarm->value = '1';
  $export['comment_preview_ns_footer_content'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_ns_footer_content';
  $strongarm->value = 1;
  $export['comment_subject_field_ns_footer_content'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_ns_footer_content';
  $strongarm->value = array();
  $export['menu_options_ns_footer_content'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_ns_footer_content';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_ns_footer_content'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_ns_footer_content';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_ns_footer_content'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_ns_footer_content';
  $strongarm->value = 0;
  $export['node_submitted_ns_footer_content'] = $strongarm;

  return $export;
}
