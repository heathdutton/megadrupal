<?php
/**
 * @file
 * civicrm_multiday_event.strongarm.inc
 */

/**
 * Implementation of hook_strongarm().
 */
function civicrm_multiday_event_strongarm() {
  $export = array();

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_civicrm_multiday_event';
  $strongarm->value = 0;
  $export['comment_anonymous_civicrm_multiday_event'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_civicrm_multiday_event';
  $strongarm->value = '0';
  $export['comment_civicrm_multiday_event'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_civicrm_multiday_event';
  $strongarm->value = 1;
  $export['comment_default_mode_civicrm_multiday_event'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_civicrm_multiday_event';
  $strongarm->value = '50';
  $export['comment_default_per_page_civicrm_multiday_event'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_civicrm_multiday_event';
  $strongarm->value = 1;
  $export['comment_form_location_civicrm_multiday_event'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_civicrm_multiday_event';
  $strongarm->value = '1';
  $export['comment_preview_civicrm_multiday_event'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_civicrm_multiday_event';
  $strongarm->value = 1;
  $export['comment_subject_field_civicrm_multiday_event'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_civicrm_multiday_event';
  $strongarm->value = array();
  $export['menu_options_civicrm_multiday_event'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_civicrm_multiday_event';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_civicrm_multiday_event'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_civicrm_multiday_event';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_civicrm_multiday_event'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_civicrm_multiday_event';
  $strongarm->value = '1';
  $export['node_preview_civicrm_multiday_event'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_civicrm_multiday_event';
  $strongarm->value = 0;
  $export['node_submitted_civicrm_multiday_event'] = $strongarm;

  return $export;
}
