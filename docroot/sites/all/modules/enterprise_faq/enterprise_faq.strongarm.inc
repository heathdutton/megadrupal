<?php
/**
 * @file
 * enterprise_faq.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function enterprise_faq_strongarm() {
  $export = array();

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_enterprise_faq';
  $strongarm->value = 0;
  $export['comment_anonymous_enterprise_faq'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_enterprise_faq';
  $strongarm->value = 1;
  $export['comment_default_mode_enterprise_faq'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_enterprise_faq';
  $strongarm->value = '50';
  $export['comment_default_per_page_enterprise_faq'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_enterprise_faq';
  $strongarm->value = '0';
  $export['comment_enterprise_faq'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_enterprise_faq';
  $strongarm->value = 1;
  $export['comment_form_location_enterprise_faq'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_enterprise_faq';
  $strongarm->value = '1';
  $export['comment_preview_enterprise_faq'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_enterprise_faq';
  $strongarm->value = 1;
  $export['comment_subject_field_enterprise_faq'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_enterprise_faq';
  $strongarm->value = array();
  $export['menu_options_enterprise_faq'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_enterprise_faq';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_enterprise_faq'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_enterprise_faq';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_enterprise_faq'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_enterprise_faq';
  $strongarm->value = '1';
  $export['node_preview_enterprise_faq'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_enterprise_faq';
  $strongarm->value = 0;
  $export['node_submitted_enterprise_faq'] = $strongarm;

  return $export;
}
