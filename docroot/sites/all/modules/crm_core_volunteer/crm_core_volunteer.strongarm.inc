<?php
/**
 * @file
 * crm_core_volunteer.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function crm_core_volunteer_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_cmcv_volunteer';
  $strongarm->value = 0;
  $export['comment_anonymous_cmcv_volunteer'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_cmcv_volunteer';
  $strongarm->value = '1';
  $export['comment_cmcv_volunteer'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_cmcv_volunteer';
  $strongarm->value = 1;
  $export['comment_default_mode_cmcv_volunteer'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_cmcv_volunteer';
  $strongarm->value = '50';
  $export['comment_default_per_page_cmcv_volunteer'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_cmcv_volunteer';
  $strongarm->value = 1;
  $export['comment_form_location_cmcv_volunteer'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_cmcv_volunteer';
  $strongarm->value = '1';
  $export['comment_preview_cmcv_volunteer'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_cmcv_volunteer';
  $strongarm->value = 1;
  $export['comment_subject_field_cmcv_volunteer'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'crm_core_profile_node_type_enabled_cmcv_volunteer';
  $strongarm->value = 1;
  $export['crm_core_profile_node_type_enabled_cmcv_volunteer'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'crm_core_profile_node_type_label_cmcv_volunteer';
  $strongarm->value = 'Volunteer Form';
  $export['crm_core_profile_node_type_label_cmcv_volunteer'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'crm_core_profile_node_type_profile_cmcv_volunteer';
  $strongarm->value = 'ao_volunteer_form';
  $export['crm_core_profile_node_type_profile_cmcv_volunteer'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__cmcv_volunteer';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => TRUE,
      ),
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'rss' => array(
        'custom_settings' => FALSE,
      ),
      'search_index' => array(
        'custom_settings' => FALSE,
      ),
      'search_result' => array(
        'custom_settings' => FALSE,
      ),
      'diff_standard' => array(
        'custom_settings' => FALSE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '0',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__cmcv_volunteer'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_cmcv_volunteer';
  $strongarm->value = array(
    0 => 'main-menu',
  );
  $export['menu_options_cmcv_volunteer'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_cmcv_volunteer';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_cmcv_volunteer'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_cmcv_volunteer';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_cmcv_volunteer'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_cmcv_volunteer';
  $strongarm->value = '1';
  $export['node_preview_cmcv_volunteer'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_cmcv_volunteer';
  $strongarm->value = 0;
  $export['node_submitted_cmcv_volunteer'] = $strongarm;

  return $export;
}
